#pragma once

#include<map>
#include<time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <limits>
#include<fstream>
#include <iostream>
#include <limits>
#include <string>
#include<fstream>

#include <type_traits>

#include"AlgoTools.h"


enum PatternState :int
{
	Free = 0,
	My,
	Enemy,
	Blocked
};


static const vector<PatternState> ALL_PATTERN_STATES = {
	PatternState::Free,
	PatternState::My,
	PatternState::Enemy,
	PatternState::Blocked
};

/*
struct PatternSet
{
	int patternLength;
	map<vector<PatternState>, double> patterns;

	PatternSet()
	{

	};

	PatternSet(int length)
	{
		srand(time(NULL));
		this->patternLength = length;

		vector<vector<PatternState>> all_patterns = generatePatterns(length);
		for (auto p : all_patterns)
			this->patterns[p] = rand() % 100;//  0;
	};

	vector<vector<PatternState>> generatePatterns(int n)
	{

		vector<vector<PatternState>> temp, patterns;
		if (n > 1)
		{
			temp = generatePatterns(n - 1);
			for (auto p : temp)
				for (auto s : ALL_PATTERN_STATES)
				{
					vector<PatternState> cur = p;
					cur.insert(cur.begin(), s);
					patterns.push_back(cur);
				}
		}
		else
		{
			for (auto s : ALL_PATTERN_STATES)
			{
				vector<PatternState> cur;
				cur.insert(cur.begin(), s);
				patterns.push_back(cur);
			}
		}
		return patterns;
	}

	static void writePattern(vector<PatternSet> inpP);
	static vector<PatternSet> readPattern(int n);

	static vector<PatternState> randomPattern(int len);
	
};
*/