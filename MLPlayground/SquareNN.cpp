#include "SquareNN.h"

double SquareNN::evaluateCell(Position position, Desk* desk)
{

	Pattern pattern = getSquarePattern(position, desk);
	//pattern to input

	MatrixXd inputMtr = MatrixXd::Zero(pattern.size(), 4);
	for (int i = 0; i < pattern.size(); i++)
		inputMtr(i, int(pattern[i])) = 1.0;

	inputMtr = inputMtr.reshaped<RowMajor>(pattern.size() * 4, 1);

	MatrixXd output = trainNet.predict(inputMtr);
	//cout << "\t" << output << endl;
	return output(0, 0);

}

Pattern SquareNN::getSquarePattern(Position position, Desk* desk)
{
	int linesize = desk->getlinesize();

	Pattern pattern;

	for (int i = -linesize; i < linesize + 1; i++)
	for (int j = -linesize; j < linesize + 1; j++)
	{
		Position curPosition(position.row + i, position.col + j );

		if (desk->isInsideDesk(curPosition))
		{
			if (!(i == 0 && j==0))
				pattern.push_back(getPatternState(desk->getstate(curPosition)));
		}
		else
			pattern.push_back(PatternState::Blocked);
	}
	return pattern;
}

void SquareNN::createNetwork(string filename)
{
	//sigmoid 200 200 200
    Layer* layer1 = new FullyConnected<Sigmoid>(120*4, 400);
   // Layer* layer2 = new FullyConnected<Sigmoid>(200, 200);
   // Layer* layer3 = new FullyConnected<Sigmoid>(200, 200);
   // Layer* layer4 = new FullyConnected<Sigmoid>(200, 200);
   // Layer* layer5 = new FullyConnected<Sigmoid>(200, 200);
    Layer* layer6 = new FullyConnected<Sigmoid>(400, 100);
   // Layer* layer7 = new FullyConnected<Sigmoid>(100, 100);
    Layer* layer8 = new FullyConnected<Sigmoid>(100, 40);
    Layer* layer9 = new FullyConnected<Identity>(40, 1);
    this->trainNet.add_layer(layer1);
  //  this->trainNet.add_layer(layer2);
   // this->trainNet.add_layer(layer3);
   // this->trainNet.add_layer(layer4);
   // this->trainNet.add_layer(layer5);
    this->trainNet.add_layer(layer6);
   // this->trainNet.add_layer(layer7);
    this->trainNet.add_layer(layer8);
    this->trainNet.add_layer(layer9);


    this->trainNet.set_output(new RegressionMSE());
    this->trainNet.init(0, 0.01, 125);
    saveNetwork(filename);
}

void SquareNN::trainEvaluateCell(Position position, Desk* desk)
{
	Pattern pattern = getSquarePattern(position, desk);
	//pattern to input
	MatrixXd inputMtr = MatrixXd::Zero(pattern.size(), 4);
	for (int i = 0; i < pattern.size(); i++)
		inputMtr(i, int(pattern[i])) = 1.0;

	inputMtr = inputMtr.reshaped<RowMajor>(pattern.size() * 4, 1);

	MatrixXd output = trainNet.predict(inputMtr);
	double result = output(0, 0);

	inputLogs.push_back(inputMtr);
	outputLogs.push_back(result);
	maxNextLogs.push_back(findMaxNext(position, desk));

}

SquareNN::SquareNN(int id, bool isTrainMode, string fileName, bool loadNetwork)
	:AbstractNNPlayer(id, isTrainMode, fileName)
{
	if (!loadNetwork)
		this->createNetwork(fileName);

	this->loadNetwork(fileName);
}
