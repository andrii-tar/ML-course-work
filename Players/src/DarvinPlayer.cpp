#include "DarvinPlayer.h"

//#include"Pattern.h"


DarvinPlayer::DarvinPlayer(int id,PatternSet patternW)
	:IPlayer(id)
{
	this->patternWeight = patternW;
}





double DarvinPlayer::getCellValue(Position position, Desk* desk)
{
	double val = 0;
	//star shape based checking
	// for all directions get pattern weight
	// sum up
	Position checkPosition;
	vector<PatternState> curPattern;

	int line = desk->getlinesize();
	int h, w;
	desk->getdesksize(h, w);

	for (int drow = -1; drow < 2; drow++)
	{
		for (int dcol = -1; dcol < 2; dcol++)
		{
			curPattern.clear();
			for (int i = 1; i <= line; i++)
			{
				checkPosition = position;
				checkPosition.row += i * drow;
				checkPosition.col += i * dcol;
				if (checkPosition.row >= 0 && checkPosition.row < h
					&& checkPosition.col >= 0 && checkPosition.col < w)
				{
					curPattern.push_back(getPatternState(desk->getstate(checkPosition)));
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





Position DarvinPlayer::makemove2(Desk* desk)
{
	// once copy whole board
	// calc all values, sort by priority
	// in loop:
	//		make/get move and update nearest values, sort array
	 



	// from rand algo...
	Position pos;
	int x, y;
	x = y = -1;
	int h, w;
	desk->getdesksize(h, w);
	srand(time(NULL));

	vector<Position> empty_cells;
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
		{
			pos = Position(i, j);
			if (desk->getstate(pos) == State::Empty)
				empty_cells.push_back(pos);
		}

	int idx = 0;
	int val = 0;
	for (int i = 0; i < empty_cells.size(); i++)
	{
		//cout << "garvin player make move " << i <<"\t"<<empty_cells.size()<< endl;
		if (getCellValue(empty_cells[i], desk) > val)
		{
			idx = i;
			val = getCellValue(empty_cells[i], desk);
		}
	}

	//Sleep(200);
	return empty_cells[idx];
}

Position DarvinPlayer::makeMove(Desk* desk)
{
	Position pos;

	this->initValues(desk);

	sort(this->vCells.begin(), this->vCells.end(), &compCellFunc);
	pos=vCells.back().pos;

	vCells.pop_back();

	return pos;
}

void DarvinPlayer::initValues(Desk* desk)
{
	int linesize = desk->getlinesize();

	if (!initiated)
	{
		int h, w;
		desk->getdesksize(h, w);
		this->cellWeights = vector<vector<double>>(h, vector<double>(w, 0));

		for (int row = 0, idx=0; row < h; row++)
		{
			for (int col = 0; col < w; col++)
			{
				Position p;
				p.row = row;
				p.col = col;
				cellWeights[row][col] = getCellValue(p, desk);

				EvoCell c;
				c.value = &cellWeights[row][col];
				c.pos = p;
				vCells.push_back(c);
			}
		}

		initiated = 1;
	}
}

void DarvinPlayer::updatePlayer(Position pos, Desk* desk)
{
	int linesize = desk->getlinesize();

	int row, col, col1, col2, row1, row2;
	int h, w;

	desk->getdesksize(h, w);

	col1 = max(0, pos.col - linesize + 1);
	col2 = min(w, pos.col + linesize);

	row1 = max(0, pos.row - linesize + 1);
	row2 = min(h, pos.row + linesize);

	for (row = row1; row < row2; row++)
		for (col = col1; col < col2; col++)
		{
			Position p(row=row, col=col);
			cellWeights[row][col] = getCellValue(p, desk);
		}
}

PatternState DarvinPlayer::getPatternState(State state)
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

double DarvinPlayer::getPatternWeight(vector<PatternState> pattern)
{
	return this->patternWeight.patterns[pattern];
}




