// **************************************************************************
//  File       [pagerank.cpp]
//  Author     [Hao-Chien Wang]
//  Synopsis   [The implementation for the FindPageRank Class]
//  Modify     [2020/11/28 Hao-Chien Wang]
// **************************************************************************

#include "pagerank.h"

Page::Page(int index,
		   int totalLink,
		   vector<int> & links):
				_index(index),
				_totalLink(totalLink),
				_links(links)
{
	rank = 1.0/N;
}
 

bool Page::operator<(const Page & p) const
{
	return this->rank < p.rank;
}

bool Page::operator==(const Page & p) const
{
	return this->rank == p.rank;
}

FindPageRank::FindPageRank(const double DIFF, const double D):
	_DIFF(DIFF), 
	_d(D)
{
	_pageList = new Page* [N]; // An array saving all the Page instances.
	vector <int> linkList;     // a list of all the pages it links to.
	vector <string> words;     // A vector saving all the words it mentioned.
	string filePath;		   // Path to the page file
	fstream fin; 
	
	// read input file
	string buffer;
	for(int i = 0; i<(N-N_ABSENT); i++)
	{
		// initialize for each page file
		filePath =   string(PAGEPATH) + string("page") + to_string(i);
		fin.open(filePath);
		linkList.clear();
		words.clear();

		// Push each linked page into the vector
		getline(fin, buffer);
		while( buffer != PAGELIST_END )
		{
			linkList.push_back(stoi(buffer.substr(4)));
			getline(fin, buffer);
		}

		// add words to wordlist
		getline(fin, buffer);
		fin.close();
		_sepWords(buffer, words);
		for(int j = 0; j<int(words.size()); j++)
		{
			if (_wordlist.find(words[j]) == _wordlist.end() )
				_wordlist[words[j]] = new set<int>;
			_wordlist[words[j]]->insert(i);
		}

		// use the information above to create a new Page
		_pageList[i] = new Page(i, linkList.size(), linkList);

	}

	// create empty pages for the page that does not have a link
	// assuming that they start from N-N_ABSENT
	for(int i = (N-N_ABSENT); i<N; i++)
	{
		vector<int> emptyvec;
		_pageList[i] = new Page(i, 0, emptyvec);
	}


	// write reverseindex.txt
	fstream fout;
	fout.open(OUTPUT_REVIND_PATH, ios::out);
	map< string, set<int>* >::iterator iter;
	//if (_wordlist.begin()->first == string(""))
		//cout << "frick" << endl;
	for(iter = _wordlist.begin(); iter != _wordlist.end(); iter++)
	{
		fout << iter->first;
		set<int>::iterator iterw;
		for(iterw = iter->second->begin(); iterw != iter->second->end(); iterw++)
		{
			fout << " page" << *iterw ;
		}
		fout << endl;
	}
	fout.close();
}

FindPageRank::~FindPageRank()
{
	for ( int i=0; i<N; i++ )
		delete _pageList[i];
	delete [] _pageList;

	map< string, set<int>* >::iterator iter;
	for(iter = _wordlist.begin(); iter != _wordlist.end(); iter++)
		delete iter->second;
}

void FindPageRank::_sepWords(string str, vector<string>& wordlist)
{
	wordlist.clear();
	string word;
	size_t ts = 0;
	size_t found;
	found = str.find(" ");
	while ( found != string::npos )
	{
		if (found != ts)
			wordlist.push_back(str.substr(ts, found-ts));
		ts = found+1;
		found = str.find(" ", ts);
	}
	if (str.substr(ts, found-ts) != "")
	{
		wordlist.push_back(str.substr(ts, found-ts));
	}
}

void FindPageRank::iterate()
{
	double diff;
	vector<double> nextRank(N, 0);
	int count=0;

	do {
		diff = 0;
		fill(nextRank.begin(), nextRank.end(), 0);
		for(int i = 0; i<N; i++)
		{
			double t = _pageList[i]->rank * _d / _pageList[i]->totalLink();
			const vector<int>& outlink = _pageList[i]->links(); 
			for(int j = 0; j<int(outlink.size()); j++)
			{
				nextRank[outlink[j]] += t;
			}
		}
		for(int i = 0; i<N; i++)
		{
			nextRank[i] += ((1.0-_d)/N);
			if ( _pageList[i]->rank >= nextRank[i] )
				diff += ( _pageList[i]->rank - nextRank[i] );
			else
				diff += ( nextRank[i] - _pageList[i]->rank );
		}
		for(int i = 0; i<N; i++)
		{
			_pageList[i]->rank = nextRank[i];
		}
	count++;
		
	} while ( diff >= _DIFF );
}

void FindPageRank::printPR()
{
	fstream fout;
	fout.open(
			string(OUTPUT_PATH) + 
			string("pr_") + 
			string(XLEN - to_string(int(_d*100)).length(), '0') +  
			to_string(int(_d*100)) + 
			string("_") + 
			string(YLEN - to_string(int(_DIFF*1000)).length(), '0') +  
			to_string(int(_DIFF*1000)) + 
			string(".txt"), ios::out);

	vector<Page> page_sorted;
	for(int i = 0; i<N; i++)
		page_sorted.push_back(*_pageList[i]);
	sort(page_sorted.begin(), page_sorted.end());
	reverse(page_sorted.begin(), page_sorted.end());

	for(int i = 0; i<N; i++)
	{
		fout << left << setw(9) << (string("page")+to_string(page_sorted[i].index()));
		fout << right << setw(6) << page_sorted[i].totalLink(); 
		fout << "     " << fixed << setprecision(7) << page_sorted[i].rank << endl;
	}
	fout.close();
}

void FindPageRank::search()
{
	fstream fin(SEARCH_INPUT_PATH);
	fstream fout;
	fout.open(
			string(OUTPUT_PATH) + 
			string("result_") + 
			string(XLEN - to_string(int(_d*100)).length(), '0') +  
			to_string(int(_d*100)) + 
			string("_") + 
			string(YLEN - to_string(int(_DIFF*1000)).length(), '0') +  
			to_string(int(_DIFF*1000)) + 
			string(".txt"), ios::out);
	string buffer;
	vector<string> words;
	vector<Page> results;
	while ( getline(fin, buffer) )
	{
		// get rid of \r and \n that are created on windows text files
		buffer.erase(remove(buffer.begin(), buffer.end(), '\r'), buffer.end()); 
		buffer.erase(remove(buffer.begin(), buffer.end(), '\n'), buffer.end()); 
		words.clear();
		_sepWords(buffer, words);
		results.clear();
		if (words.size() == 1)
		{
			if ( _wordlist.find(words[0]) == _wordlist.end() )
				fout << "none" << endl;
			else
			{
				set<int>::iterator iter;
				for(iter = _wordlist[words[0]]->begin(); iter != _wordlist[words[0]]->end(); iter++)
					results.push_back(*_pageList[*iter]);
				sort(results.begin(), results.end());
				reverse(results.begin(), results.end());
				fout << "page" << results[0].index();
				for(int i = 1; i<min(int(results.size()), 10); i++)
				{
					fout << " page" << results[i].index();
				}
				fout << endl;
			}
		}
		else
		{
			// find union
			set<int> uni;
			vector<string>::iterator viter;
			set<int>::iterator siter;
			for (viter = words.begin(); viter != words.end(); viter++)
			{
				if ( _wordlist.find(*viter) != _wordlist.end() )
					for (siter = _wordlist[*viter]->begin(); 
						siter != _wordlist[*viter]->end(); siter++)
						uni.insert(*siter);
			}
			for (siter = uni.begin(); siter != uni.end(); siter++)
				results.push_back(*_pageList[*siter]);
			// print OR result
			if (results.empty())
				fout << "OR none" << endl;
			else
			{

				sort(results.begin(), results.end());
				reverse(results.begin(), results.end());
				fout << "OR page" << results[0].index();
				for(int i = 1; i<min(int(results.size()), 10); i++)
				{
					fout << " page" << results[i].index();
				}
				fout << endl;
			}

			// find intersection
			 results.clear();
			
			// find smallest set
			set<int>* smallest = _wordlist[words[0]];
			set<int>::size_type smallest_size = smallest->size();
			for (viter = words.begin(); viter != words.end(); viter++)
			{
				if ( _wordlist.find(*viter) != _wordlist.end() )
					if (_wordlist[*viter]->size() < smallest_size ) 
					{
						smallest =_wordlist[*viter];
						smallest_size = smallest->size();
					}
 
			}
			set<int> intersection = *smallest;
			vector<int> deleteList;
			// remove elements if they are not in all sets
			for (siter = intersection.begin(); siter != intersection.end(); siter++)
				for (viter = words.begin(); viter != words.end(); viter++)
					if ((_wordlist.find(*viter) == _wordlist.end()) ||
						(_wordlist[*viter]->find(*siter) == _wordlist[*viter]->end()) ) 
						// if the word is not in another set
						{
							deleteList.push_back(*siter);
							break;
						}
			for(int i = 0; i<int(deleteList.size()); i++)
				intersection.erase(deleteList[i]);

			// print AND result
			results.clear();
			for (siter = intersection.begin(); siter != intersection.end(); siter++)
				results.push_back(*_pageList[*siter]);
			if (results.empty())
				fout << "AND none" << endl;
			else
			{
				sort(results.begin(), results.end());
				reverse(results.begin(), results.end());
				fout << "AND page" << results[0].index();
				for(int i = 1; i<min(int(results.size()), 10); i++)
				{
					fout << " page" << results[i].index();
				}
				fout << endl;
			}
		}
	}
	fout.close();
}
