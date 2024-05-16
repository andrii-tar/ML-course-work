#pragma once
#include "GameEnv.h"
#include"Players.h"

#include"Eigen/Dense"
#include"MiniDNN.h"
#include<string>

#include"AbstractNNPlayer.h"

using namespace std;
using namespace MiniDNN;
using namespace Eigen;



class StarNN :
    public AbstractNNPlayer
{

	Position getDelta(MainDirection direction);

	Pattern getLinePattern(Position position, Desk* desk, MainDirection direction);

	double evaluateCell(Position position, Desk* desk);


	void createNetwork(string filename);

	void trainEvaluateCell(Position position, Desk* desk);


	string filename = "star_nn_10.dnn";
public:
	StarNN(int id, bool isTrainMode = false, string fileName = "star_nn_.dnn", bool loadNetwork = true);
};

