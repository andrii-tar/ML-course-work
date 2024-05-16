#pragma once

#include"GameEnv.h"
class RandomPlayer : public IPlayer
{
public:
	RandomPlayer(int id);
	Position makeMove(Desk* desk);
};
