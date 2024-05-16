#pragma once
#include<vector>

#include<omp.h>
#include"Players.h"

using namespace std;
class Tournament
{



    static pair<int, int> runSingleGame(ShortPatternSet pattern1, ShortPatternSet pattern2, Config& config);
public:
	static vector<vector<int>> tournamentPiramide(vector<ShortPatternSet> patternSets, bool hidden = true);
	


};

