#include "StarNN.h"


Position StarNN::getDelta(MainDirection direction) {

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

Pattern StarNN::getLinePattern(Position position, Desk* desk, MainDirection direction)
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

double StarNN::evaluateCell(Position position, Desk* desk)
{
	Pattern finalPattern = {};
	for (auto dir : ALL_MAIN_DIRECTIONS)
	{
		Pattern linePattern = getLinePattern(position, desk, dir);
		finalPattern.insert(finalPattern.end(), linePattern.begin(), linePattern.end());
	}


	//pattern to input
	MatrixXd inputMtr = MatrixXd::Zero(finalPattern.size(), 4);
	for (int i = 0; i < finalPattern.size(); i++)
		inputMtr(i, int(finalPattern[i])) = 1.0;

	inputMtr = inputMtr.reshaped<RowMajor>(finalPattern.size() * 4, 1);

	MatrixXd output = trainNet.predict(inputMtr);
	//cout << "\t" << output << endl;
	return output(0, 0);

}

void StarNN::createNetwork(string filename)
{
	/*
    Layer* layer1 = new FullyConnected<Sigmoid>(40*4, 250);
    Layer* layer2 = new FullyConnected<Sigmoid>(250, 100);
    Layer* layer3 = new FullyConnected<Sigmoid>(100, 50);
    Layer* layer4 = new FullyConnected<Identity>(50, 1);
	*/
	/*
	Layer* layer1 = new FullyConnected<ReLU>(40 * 4, 160);
	Layer* layer3 = new FullyConnected<ReLU>(160, 50);
	Layer* layer4 = new FullyConnected<Identity>(50, 1);
	*/
	
	Layer* layer1 = new FullyConnected<Sigmoid>(40*4, 160);
    Layer* layer2 = new FullyConnected<Sigmoid>(160, 120);
    Layer* layer3 = new FullyConnected<Sigmoid>(120, 50);
    Layer* layer4 = new FullyConnected<Identity>(50, 1);


    this->trainNet.add_layer(layer1);
    this->trainNet.add_layer(layer2);
    this->trainNet.add_layer(layer3);
    this->trainNet.add_layer(layer4);
   // this->trainNet.add_layer(layer5);

    this->trainNet.set_output(new RegressionMSE());
    this->trainNet.init(0, 0.01, 125);
    saveNetwork(filename);
}

void StarNN::trainEvaluateCell(Position position, Desk* desk)
{
	

	Pattern finalPattern;
	for (auto dir : ALL_MAIN_DIRECTIONS)
	{
		Pattern linePattern = getLinePattern(position, desk, dir);
		finalPattern.insert(finalPattern.end(), linePattern.begin(), linePattern.end());
	}

	//pattern to input
	MatrixXd inputMtr = MatrixXd::Zero(finalPattern.size(), 4);
	for (int i = 0; i < finalPattern.size(); i++)
		inputMtr(i, int(finalPattern[i])) = 1.0;

	inputMtr = inputMtr.reshaped<RowMajor>(finalPattern.size() * 4, 1);

	MatrixXd output = trainNet.predict(inputMtr);
	double result = output(0, 0);

	inputLogs.push_back(inputMtr);
	outputLogs.push_back(result);
	maxNextLogs.push_back(findMaxNext(position, desk));

}

StarNN::StarNN(int id, bool isTrainMode, string fileName, bool loadNetwork)
	:AbstractNNPlayer(id, isTrainMode, fileName)
{
	if (!loadNetwork)
		this->createNetwork(fileName);

	this->loadNetwork(fileName);

	/*
	40250
25100
8080
4050
51
	*/
}