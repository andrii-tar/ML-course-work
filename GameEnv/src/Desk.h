#pragma once
#include<iostream>
#include<vector>

#include <windows.h>

#include"Config.h"

using namespace std;

enum State : int
{
	Empty = 0,
	Nought = 1,
	Cross = 2,
	PlayedNought = 3,
	PlayedCross = 4
};

class Desk
{
	int width, height;
	int nought_line_cnt, cross_line_cnt;
	int linesize;
	vector<vector<State>> desk;

	void setstate(Position pos, State newstate);
	void cross(Position pos);

public:
	Desk(Config newconfig);
	
	State getstate(int row, int col);
	State getstate(Position pos);
	vector<vector<State>> showdesk();
	void getdesksize(int& h, int& w);
	int getheight();
	int getwidth();
	int getlinesize(); 
	
	bool isInsideDesk(Position position);

	friend class Game;
};

