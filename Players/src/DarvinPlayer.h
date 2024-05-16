#pragma once

//#include"IPlayer.h"
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

//#include"Evolution.h"

#include"Pattern.h"


struct EvoCell
{
	Position pos;
	double* value;

	bool operator < (EvoCell const &cell)
	{
		return *(value) < *(cell.value);
	}
};

static bool compCellFunc(EvoCell c1, EvoCell c2)
{
	return (*(c1.value) < *(c2.value));
}

class DarvinPlayer :public IPlayer
{
	vector<EvoCell> vCells;
	vector<vector<double>> cellWeights;

	bool initiated = 0;
public:
	DarvinPlayer(int id, PatternSet patternW);


	Position makeMove(Desk* desk);
	Position makemove2(Desk* desk);

	PatternSet patternWeight;

	void initValues(Desk* desk);

	void updatePlayer(Position pos, Desk* desk);

	//void distances();

	PatternState getPatternState(State state);


	
	double getPatternWeight(vector<PatternState> pattern);


	double getCellValue(Position position, Desk* desk);
};


