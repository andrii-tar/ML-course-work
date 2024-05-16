#include "pch.h"
#include "GreedyAlgoPlayer.h"

double GreedyAlgoPlayer::evaluateCell(Position position, Desk* desk)
{
	int longestLine = 0;

	//horizontal
	int tempLine = 1;
	for (int row = position.row + 1; row <= position.row + 5; row++)
	{
		if (row >= 0 && row < desk->getheight() && getPatternState(desk->getstate(Position(row, position.col))) == PatternState::My)
			tempLine++;
		else
			break;
	}
	longestLine = max(longestLine, tempLine);
	tempLine = 1;

	for (int row = position.row - 1; row >= position.row - 5; row--)
	{
		if (row >= 0 && row < desk->getheight() && getPatternState(desk->getstate(Position(row, position.col))) == PatternState::My)
			tempLine++;
		else
			break;
	}

	longestLine = max(longestLine, tempLine);

	//vertical
	tempLine = 1;
	for (int col = position.col + 1; col <= position.col + 5; col++)
	{
		if (col >= 0 && col < desk->getwidth() && getPatternState(desk->getstate(Position(position.row, col))) == PatternState::My)
			tempLine++;
		else
			break;
	}
	longestLine = max(longestLine, tempLine);
	tempLine = 1;

	for (int col = position.col - 1; col >= position.col - 5; col--)
	{
		if (col >= 0 && col < desk->getwidth() && getPatternState(desk->getstate(Position(position.row, col))) == PatternState::My)
			tempLine++;
		else
			break;
	}

	longestLine = max(longestLine, tempLine);

	//diag down
	tempLine = 1;
	for (int col = position.col + 1, row = position.row + 1;
		col <= position.col + 5 && row <= position.row + 5; col++, row++)
	{
		if (row >= 0 && row < desk->getheight() && col >= 0 && col < desk->getwidth() && getPatternState(desk->getstate(Position(row, col))) == PatternState::My)
			tempLine++;
		else
			break;
	}
	longestLine = max(longestLine, tempLine);
	tempLine = 1;

	for (int col = position.col - 1, row = position.row - 1;
		col >= position.col - 5 && row >= position.row - 5; col--, row--)
	{
		if (row >= 0 && row < desk->getheight() && col >= 0 && col < desk->getwidth() && getPatternState(desk->getstate(Position(row, col))) == PatternState::My)
			tempLine++;
		else
			break;
	}

	longestLine = max(longestLine, tempLine);

	//diag up
	tempLine = 1;
	for (int col = position.col + 1, row = position.row - 1;
		col <= position.col + 5 && row >= position.row - 5; col++, row--)
	{
		if (row >= 0 && row < desk->getheight() && col >= 0 && col < desk->getwidth() && getPatternState(desk->getstate(Position(row, col))) == PatternState::My)
			tempLine++;
		else
			break;
	}
	longestLine = max(longestLine, tempLine);
	tempLine = 1;

	for (int col = position.col - 1, row = position.row + 1;
		col >= position.col - 5 && row <= position.row + 5; col--, row++)
	{
		if (row >= 0 && row < desk->getheight() && col >= 0 && col < desk->getwidth() && getPatternState(desk->getstate(Position(row, col))) == PatternState::My)
			tempLine++;
		else
			break;
	}

	longestLine = max(longestLine, tempLine);

	return longestLine;
}

void GreedyAlgoPlayer::initValues(Desk* desk)
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
				cellWeights(row, col) = evaluateCell(p, desk);
			}
		}

		initialized = true;
	}
}

GreedyAlgoPlayer::GreedyAlgoPlayer(int id)
	:IPlayer(id)
{
}

/*
PatternState GreedyAlgoPlayer::getPatternState(State state)
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
*/
Position GreedyAlgoPlayer::makeMove(Desk* desk)
{
	Position position;
	cellWeights.maxCoeff(&position.row, &position.col);

	//cout <<"max coef: "<< position.row << " " << position.col << "\tval"<<cellWeights(position.row, position.col)<<endl;

	cellWeights(position.row, position.col) = -0.5 * DBL_MAX;
	return position;
}

void GreedyAlgoPlayer::updatePlayer(Position position, Desk* desk)
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
				cellWeights(row, col) = evaluateCell(p, desk);
		}

}
