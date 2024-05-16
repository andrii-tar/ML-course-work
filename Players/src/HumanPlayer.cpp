#include "pch.h"
#include "HumanPlayer.h"



HumanPlayer::HumanPlayer(int id)
	:IPlayer(id)
{
}

Position HumanPlayer::makeMove(Desk* desk)
{
	Position pos;
	int x, y;
	x = y = 0;
	bool b = 0;

	cout << "Player " << this->player_id << " turn: ";
	cin >> y >> x;

	x--;
	y--;
	pos.col = x;
	pos.row = y;
	return pos;
}