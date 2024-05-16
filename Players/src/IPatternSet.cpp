#include "IPatternSet.h"

IPatternSet::IPatternSet(int patternLength)
{
	this->patternLength = patternLength;
}

int IPatternSet::getNumPatterns()
{
	return weightedPatterns.size();
}
