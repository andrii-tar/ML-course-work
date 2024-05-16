#include "pch.h"
#include "RandomPlayer.h"


RandomPlayer::RandomPlayer(int id)
	:IPlayer(id)
{
}


Position RandomPlayer::makeMove(Desk* desk)
{
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

	int i = rand() % empty_cells.size();


	return empty_cells[i];
}
