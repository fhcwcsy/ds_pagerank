// **************************************************************************
//  File       [main.cpp]
//  Author     [Hao-Chien Wang] (modified from the original version by Yu-Hao Ho)
//  Synopsis   [The main program of 2019 fall Algorithm PA2]
//  Modify     [2020/11/14 Hao-Chien Wang]
// **************************************************************************

#include <iostream>
#include <fstream>
#include "pagerank.h"


using namespace std;

void help_message() {
    cout << "usage: pagerank <diff> <d>" << endl;
}

int main(int argc, char* argv[])
{
    if(argc != 3) {
       help_message();
       return 0;
    }

	const double DIFF = atof(argv[1]);
	const double D = atof(argv[2]);

	FindPageRank pr(DIFF, D);
	pr.iterate();
	pr.printPR();
	pr.search();
	
    return 0;
}
