#include "AbstractNNPlayer.h"

void AbstractNNPlayer::loadNetwork(string filename)
{
	this->evalNet.read_net("models", filename);
	this->trainNet.read_net("models", filename);
}
void AbstractNNPlayer::saveNetwork(string filename)
{
	this->trainNet.export_net("models", filename);
}

double AbstractNNPlayer::findMaxNext(Position position, Desk* desk)
{
	Limits lim = calclimits(position.row, position.col, MainDirection::Diagonal_down, desk);



	int h, w;
	h = w = lim.x2 - lim.x1 + 1;

	double mx = -100000;

	MatrixXd segmentCW = cellWeights.block(lim.y1, lim.x1, h, w);
	/*
	for (int i = 0; i < h; i++) {
		
		mx = max(mx, segmentCW(i, i));
		mx = max(mx, segmentCW(i, h - i));
		mx = max(mx, segmentCW(i, 0));
		mx = max(mx, segmentCW(0, i));

	}
	return mx;
	*/
	//cout <<endl<< lim.y1 << " " << lim.x1<<" " << h << " " << w << endl;

	//cout << segmentCW << endl;

	Position maxPos;
	segmentCW.maxCoeff(&maxPos.row, &maxPos.col);
	//	cout << "found: " << maxPos.row << " " << maxPos.col << endl;
	maxPos.row += lim.y1;
	maxPos.col += lim.x1;
	//	cout << "final: " << maxPos.row << " " << maxPos.col << endl;
	double maxWeight = cellWeights(maxPos.row, maxPos.col);

	return maxWeight;
}
void AbstractNNPlayer::train()
{
	//cout << cellWeights << endl;
	if (inputLogs.empty() == false)
	{
		MatrixXd input = inputLogs.back();
		MatrixXd target(1, 1);
		double reward = 0;
		if (myScrLog.back() != 0)
			reward += myScrLog.back() * qConf.myScoreReward;
		else if (enemyScrLog.back() != 0)
			reward += enemyScrLog.back() * qConf.enemyScoreReward;
		else {

			reward += qConf.alpha * maxNextLogs.back();
		}
		target(0, 0) = reward;


		trainNet.fit(qConf.optimizer, input, target, 1, 1, 123);

		if (inputLogs.size() % qConf.learningFreq == 0) {
			saveNetwork(this->filename);
			loadNetwork(this->filename);
		}
	}
}

void AbstractNNPlayer::initValues(Desk* desk)
{
	int linesize = desk->getlinesize();

	if (!initialized)
	{
		int h, w;
		desk->getdesksize(h, w);
		this->cellWeights = MatrixXd(h, w);
		this->cellWeights = MatrixXd::Zero(h, w);

		for (int row = 0; row < h; row++)
		{
			for (int col = 0; col < w; col++)
			{
				Position p(row, col);
				double cellWeight = evaluateCell(p, desk);
				cellWeights(row, col) = cellWeight;
			}
		}
		//cout << cellWeights << endl;
		initialized = true;
	}
}
AbstractNNPlayer::AbstractNNPlayer(int id, bool isTrainMode,string fileName)
	:IPlayer(id)
{
	this->filename = fileName;
	this->isTrainMode = isTrainMode;
	
}

Position AbstractNNPlayer::makeMove(Desk* desk)
{
	if (isTrainMode)
		train();

	//cout << cellWeights << endl;
	Position position;
	cellWeights.maxCoeff(&position.row, &position.col);

	double randMoveProb = (rand() % 100 / 100.0);

	if (randMoveProb < qConf.epsilon)
	{
		int t = 10;
		while (t > 0) {
			int row = rand() % desk->getheight();
			int col = rand() % desk->getwidth();
			if (desk->getstate(row, col) == State::Empty)
			{
				position = Position(row, col);
				break;
			}
			t--;
		}
	}

	cellWeights(position.row, position.col) = -0.5 * DBL_MAX;

	if (isTrainMode)
	{
		trainEvaluateCell(position, desk);
	}
	return position;
}

void AbstractNNPlayer::updatePlayer(Position position, Desk* desk)
{
	cellWeights(position.row, position.col) = -0.5 * DBL_MAX;

	int lineSize = desk->getlinesize();

	int row, col, colFrom, colTo, rowFrom, rowTo;
	int h, w;

	desk->getdesksize(h, w);

	colFrom = max(0, position.col - lineSize + 1);
	colTo = min(w, position.col + lineSize);

	rowFrom = max(0, position.row - lineSize + 1);
	rowTo = min(h, position.row + lineSize);

	for (row = rowFrom; row < rowTo; row++)
		for (col = colFrom; col < colTo; col++)
		{
			Position p(row = row, col = col);
			if (desk->getstate(p) == State::Empty)
			{
				auto cellWeight = evaluateCell(p, desk);
				cellWeights(row, col) = cellWeight;
			}
		}
	State state = desk->getstate(position);

	bool isMyMove = (state % 2 == this->player_id % 2);

	switch (state)
	{
	case State::Nought:
	case State::Cross:
		if (isMyMove)
			myScrLog.push_back(0);
		else
			enemyScrLog.push_back(0);
		break;
	case State::PlayedNought:
	case State::PlayedCross:
		if (isMyMove)
			myScrLog.push_back(1);
		else
			enemyScrLog.push_back(1);
		break;
	default:
		break;
	}


}


