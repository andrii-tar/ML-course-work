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

#include"Players.h"


#include"IPatternSet.h"

class ShortPatternSet : public IPatternSet
{
	int patternLength;
public:
	map<Pattern, double> weightedPatterns;
	ShortPatternSet(){}
	ShortPatternSet(int patternLength);
	vector<Pattern> generatePatterns(int patternLength);
	static void savePatternSets(vector<ShortPatternSet> patternSets, string series = "def_s");
	static void saveSinglePatternSet(ShortPatternSet patternSet, string fileSufix = "", string series = "def_s");
	static vector<ShortPatternSet> loadPatternSets(int n, int patternLength, string series="def_s");
	static ShortPatternSet loadSinglePatternSet(int patternLength, string fileSufix="", string series = "def_s");

	int getNumPatterns();

};

