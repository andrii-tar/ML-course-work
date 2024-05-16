#pragma once

#include "GameEnv.h"
#include"Players.h"

#include"Eigen/Dense"
#include"MiniDNN.h"
#include<string>


using namespace std;
using namespace MiniDNN;
using namespace Eigen;


struct QConfig
{
	//double myScoreReward = 10;
	double myScoreReward = 10.0;
	double enemyScoreReward =0.0;
	//double enemyScoreReward =-20;

	double moveReward = 0.0;

	int learningFreq = 50;
	Adam optimizer = Adam(0.001);

	double epsilon = (1.0 / 200.0);
	double alpha = 0.8;
};

class AbstractNNPlayer :
    public IPlayer
{
protected:
	bool initialized = false;
	MatrixXd cellWeights;
	
	Network evalNet;
	Network trainNet;

	string filename = "any_nn_.dnn";
	QConfig qConf;

	vector<MatrixXd> inputLogs;
	vector<double> outputLogs;
	vector< double> maxNextLogs;
	vector<int> myScrLog, enemyScrLog;
	bool isTrainMode;

	virtual double evaluateCell(Position position, Desk* desk) = 0;


	virtual void createNetwork(string filename) = 0;
	void loadNetwork(string filename);

	virtual void trainEvaluateCell(Position position, Desk* desk) = 0;

	double findMaxNext(Position position, Desk* desk);

	void train();
public:
	void saveNetwork(string filename);
	void initValues(Desk* desk);
	AbstractNNPlayer(int id, bool isTrainMode = false, string fileName = "");

	Position makeMove(Desk* desk);


	void updatePlayer(Position position, Desk* desk);

};

