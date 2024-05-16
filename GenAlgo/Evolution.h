#pragma once
#include<vector>

#include"Players.h"
using namespace std;

struct EvoConfig
{
	int populationSize=25;
	int numParents=10;
	int  numCrossovers = 5;

	double mutatedValuesShare = 0.3;
	double mutationRangePercent = 0.4;
	double minMutaionRange = 2.0;
	double maxMutaionRange = 20.0;

	double minWeight = -100;
	double maxWeight = 100;

};


class Evolution
{
	vector<int> selectNextGenIdx(vector<pair<double, int>> rating);
	const int MIN_VALUE = -10000000;
	vector<pair<double, int>> ratePlayers(vector<vector<int>> tournamentTable);
	ShortPatternSet mutate(ShortPatternSet originalPatterns);
	ShortPatternSet crossover(ShortPatternSet parrentA, ShortPatternSet parrentB);


public:
	EvoConfig evoConf;
	Evolution(EvoConfig evolutionConfig);


	vector<ShortPatternSet> repopulate(vector<vector<int>> tournamentTable, vector<ShortPatternSet> oldPopulation, int logSelectedIndividuals = false);

};

