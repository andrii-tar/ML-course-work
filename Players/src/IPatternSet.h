#pragma once

#include<map>
#include"GameEnv.h"

typedef vector<PatternState> Pattern;
class IPatternSet
{
	int patternLength;
public:
	map<Pattern, double> weightedPatterns;
	IPatternSet(){}
	IPatternSet(int patternLength);
	virtual vector<Pattern> generatePatterns(int patternLength) = 0;

	int getNumPatterns();

};

