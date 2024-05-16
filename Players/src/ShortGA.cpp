#include "ShortGA.h"

void ShortGA::initValues(Desk* desk)
{
	int linesize = desk->getlinesize();

	if (!initialized)
	{
		int h, w;
		desk->getdesksize(h, w);
		this->cellWeights = MatrixXd(h, w);
		this->cellWeights = MatrixXd::Zero(h, w);

		for (int row = 0, idx = 0; row < h; row++)
		{
			for (int col = 0; col < w; col++)
			{
				Position p(row, col);
				cellWeights(row,col) = evaluateCell(p, desk);
			}
		}

		initialized = true;
	}
}

double ShortGA::evaluateCell(Position position, Desk* desk)
{
	double val = 0;
	//star shape based checking
	// for all directions get pattern weight
	// sum up

	Position checkPosition;
	Pattern curPattern;

	int lineSize = desk->getlinesize();
	int h, w;
	desk->getdesksize(h, w);

	for (int drow = -1; drow < 2; drow++)
	{
		for (int dcol = -1; dcol < 2; dcol++)
		{
			curPattern.clear();
			for (int i = 1; i <= lineSize; i++)
			{
				checkPosition = position;
				checkPosition.row += i * drow;
				checkPosition.col += i * dcol;
				if(desk->isInsideDesk(checkPosition))
				{
					curPattern.push_back(
						getPatternState(desk->getstate(checkPosition))
					);
				}
				else
				{
					curPattern.push_back(PatternState::Blocked);
				}
			}
			val += getPatternWeight(curPattern);
		}
	}
	return val;
}

PatternState ShortGA::getPatternState(State state)
{
	switch (state)
	{
	case Empty: {
		return PatternState::Free;
		break;
	}
	case PlayedNought: {
		return PatternState::Blocked;
		break;
	}
	case PlayedCross: {
		return PatternState::Blocked;
		break;
	}
	default:
	{
		if (state == this->player_id)
			return PatternState::My;
		else
			return PatternState::Enemy;
		break;
	}
	}
}

double ShortGA::getPatternWeight(Pattern pattern)
{
	return this->patternSet.weightedPatterns[pattern];
}

ShortGA::ShortGA(int id, ShortPatternSet patternSet)
	:IPlayer(id)
{
	this->patternSet = patternSet;
}

Position ShortGA::makeMove(Desk* desk)
{
	Position position;
	cellWeights.maxCoeff(&position.row, &position.col);

	//cout <<"max coef: "<< position.row << " " << position.col << "\tval"<<cellWeights(position.row, position.col)<<endl;

	cellWeights(position.row, position.col) = -0.5 * DBL_MAX;
	return position;
}

void ShortGA::updatePlayer(Position position, Desk* desk)
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
			if(desk->getstate(p) ==State::Empty)
				cellWeights(row,col) = evaluateCell(p, desk);
		}

}
