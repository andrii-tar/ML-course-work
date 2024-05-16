#pragma once
#include "AbstractNNPlayer.h"
class SquareNN :
    public AbstractNNPlayer
{
	double evaluateCell(Position position, Desk* desk);

	Pattern getSquarePattern(Position position, Desk* desk);

	void createNetwork(string filename);

	void trainEvaluateCell(Position position, Desk* desk);


	string filename = "square_nn_10.dnn";

public:
	SquareNN(int id, bool isTrainMode = false, string fileName = "square_nn_.dnn", bool loadNetwork = true);

};

