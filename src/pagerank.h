// **************************************************************************
//  File       [pagerank.h]
//  Author     [Hao-Chien Wang]
//  Synopsis   [The header file for the FindPageRank Class]
//  Modify     [2020/11/27 Hao-Chien Wang]
// **************************************************************************

#ifndef _PAGERANK_H
#define _PAGERANK_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <set>
#include <vector>

#define PAGEPATH "./input/web-search-files2/"
#define SEARCH_INPUT_PATH "./input/list.txt"
#define OUTPUT_REVIND_PATH "./reveseindex.txt" 
#define OUTPUT_PATH "./" 
#define PAGELIST_END "---------------------"
#define N 501
#define N_ABSENT 1
#define XLEN 2
#define YLEN 3

using namespace std;

class Page
{
	private:
		int _index;
		int _totalLink;
		vector <int> _links;
	public:
		double rank;
		int totalLink() { return _totalLink; };
		int index() { return _index; };
		const vector<int>& links() { return _links; }
		
		Page(int index,
			 int totalLink, 
			 vector<int> & links);

		~Page() { };
		bool operator<(const Page & p) const;
		bool operator==(const Page & p) const;
};

class FindPageRank
{
	private:
		const double _DIFF;
		const double _d;
		map < string, set<int>* >	_wordlist;
		Page** _pageList;
		void _sepWords(string str, vector<string>& wordlist);
	public:
		FindPageRank(const double DIFF, const double D);
		~FindPageRank();
		void iterate();
		void printPR();
		void search();
};

#endif
