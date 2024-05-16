#pragma once
#include"GameEnv.h"
#include"Eigen/Dense"

using namespace Eigen;


class GreedyAlgoPlayer :
    public IPlayer
{
	bool initialized = false;

	double evaluateCell(Position position, Desk* desk);
	MatrixXd cellWeights;

public:
	void initValues(Desk* desk);
	GreedyAlgoPlayer(int id);

	Position makeMove(Desk* desk);


	void updatePlayer(Position position, Desk* desk);

};
