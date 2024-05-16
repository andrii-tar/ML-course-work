#pragma once
#include <iostream>
#include<vector>
#include<Windows.h>
#include <stdlib.h>     
#include <time.h>   
#include<algorithm>
#include<math.h>

#include<iomanip>


#include"Game.h"
#include"Pattern.h"
using namespace std;

//finish player symbol definition...
class IPlayer
{
protected:
	int player_id;
public:
	IPlayer(int id);
	~IPlayer();
	Position virtual makeMove(Desk* desk) = 0;
	void virtual initValues(Desk* desk);
	void virtual updatePlayer(Position pos, Desk* desk);

	PatternState getPatternState(State state);


};

