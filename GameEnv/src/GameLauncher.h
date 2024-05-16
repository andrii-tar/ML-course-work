#pragma once
#include"Game.h"
#include"IPlayer.h"


#include"../Utils.h"

class GameLauncher
{
private:
	Game* game;
	Position makemove(IPlayer* player, int pl_id);
	
public:
	GameLauncher(Config newconfig);
	pair<int, int> start(IPlayer* player1, IPlayer* player2);



};

