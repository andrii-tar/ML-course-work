#pragma once
#include"GameEnv.h"
class HumanPlayer :public IPlayer
{
public:
	HumanPlayer(int id);
	Position makeMove(Desk* desk);
};
