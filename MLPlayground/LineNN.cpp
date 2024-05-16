#include "LineNN.h"

Position getDelta(MainDirection direction) {

	switch (direction)
	{
	case MainDirection::Vertical:
		return Position(1, 0);
		break;
	case MainDirection::Horizontal:
		return Position(0, 1);
		break;
	case MainDirection::Diagonal_up:
		return Position(-1, 1);
		break;
	case MainDirection::Diagonal_down:
		return Position(1, 1);
		break;
	default:
		break;
	}
}
Pattern LineNN::getLinePattern(Position position, Desk* desk, MainDirection direction)
{
	int linesize = desk->getlinesize();


	Position delta = getDelta(direction);
	Pattern pattern;

	for (int i = -linesize; i < linesize + 1; i++)
	{
		Position curPosition(position.row + i * delta.row, position.col + i * delta.col);

		if (desk->isInsideDesk(curPosition))
		{
			if (i != 0)
				pattern.push_back(getPatternState(desk->getstate(curPosition)));
		}
		else
			pattern.push_back(PatternState::Blocked);
	}
	return pattern;
}

double LineNN::evaluateLine(Position position, Desk* desk, MainDirection direction)
{
	Pattern pattern = getLinePattern(position, desk, direction);
	//pattern to input
	MatrixXd inputMtr = MatrixXd::Zero(pattern.size(), 4);
	for (int i = 0; i < pattern.size(); i++)
		inputMtr(i, int(pattern[i])) = 1.0;

	inputMtr = inputMtr.reshaped<RowMajor>(pattern.size()*4, 1);

	MatrixXd output = trainNet.predict(inputMtr);
	//cout << "\t" << output << endl;
	return output(0, 0);
}


double LineNN::evaluateCell(Position position, Desk* desk)
{
	double result= -10000;

	for (auto dir : ALL_MAIN_DIRECTIONS)
	{
		double lineResult = evaluateLine(position, desk, dir);

		if (lineResult > result)
		{
			result = lineResult;
		}
	}
	return result;

}

void LineNN::createNetwork(string filename)
{
	
	/*
		Layer* layer1 = new FullyConnected<ReLU>(40, 20);
    Layer* layer2 = new FullyConnected<ReLU>(20, 10);
    Layer* layer3 = new FullyConnected<Identity>(10, 1);
    this->trainNet.add_layer(layer1);
    this->trainNet.add_layer(layer2);
    this->trainNet.add_layer(layer3);
	
	*/
	/*
	Layer* layer1 = new FullyConnected<ReLU>(40, 100);
    Layer* layer2 = new FullyConnected<ReLU>(100, 50);
    Layer* layer3 = new FullyConnected<ReLU>(50, 20);
    Layer* layer4 = new FullyConnected<Identity>(20, 1);
    this->trainNet.add_layer(layer1);
    this->trainNet.add_layer(layer2);
    this->trainNet.add_layer(layer3);
    this->trainNet.add_layer(layer4);
	*/
	
    Layer* layer1 = new FullyConnected<Sigmoid>(40, 100);
    Layer* layer2 = new FullyConnected<Sigmoid>(100,80);
    Layer* layer3 = new FullyConnected<Sigmoid>(80, 50);
    Layer* layer5 = new FullyConnected<Identity>(50, 1);
    this->trainNet.add_layer(layer1);
    this->trainNet.add_layer(layer2);
    this->trainNet.add_layer(layer3);
    this->trainNet.add_layer(layer5);

    this->trainNet.set_output(new RegressionMSE());
	this->trainNet.init(0, 0.01, 125);
	saveNetwork(filename);
}


void LineNN::trainEvaluateCell(Position position, Desk* desk)
{
	double result = -10000;
	MatrixXd finalInput;

	for (auto dir : ALL_MAIN_DIRECTIONS)
	{
		Pattern pattern = getLinePattern(position, desk, dir);

		//pattern to input
		MatrixXd inputMtr = MatrixXd::Zero(pattern.size(), 4);
		for (int i = 0; i < pattern.size(); i++)
			inputMtr(i, int(pattern[i])) = 1.0;
		inputMtr = inputMtr.reshaped<RowMajor>(pattern.size() * 4, 1);

		MatrixXd output = evalNet.predict(inputMtr);

		double lineResult = output(0, 0);
		if (lineResult > result)
		{
			result = lineResult;
			finalInput = inputMtr;
		}
	}
	inputLogs.push_back(finalInput);
	outputLogs.push_back(result);
	maxNextLogs.push_back(findMaxNext(position, desk));

}

LineNN::LineNN(int id, bool isTrainMode, string fileName, bool loadNetwork)
	:AbstractNNPlayer(id, isTrainMode, fileName)
{
	if (!loadNetwork)
		this->createNetwork(fileName);

	this->loadNetwork(fileName);
}

