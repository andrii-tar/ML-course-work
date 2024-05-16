#pragma once
#include<iostream>
#include<vector>

#include <windows.h>

#include"Desk.h"
#include"Config.h"
#include"AlgoTools.h"
using namespace std;

class Game
{
private:
	int linesize;
	int cellcnt=0;
	int p1score = 0, p2score = 0;
	bool firstwin = 0;
	bool usergameover = 0;
	int maxLinesNumber = 10000;
	vector<vector<int>>logs;

	Desk* desk;



	void update(int row, int col);

	int checkany(int* row, int* col, MainDirection dir);

	Limits calclimits2(int row, int col, MainDirection dir);

	
	void crossvert(int row, int col);
	void crosshor(int row, int col);
	void crossdiag_down(int row, int col);
	void crossdiag_up(int row, int col);
public:
	Game(Config newconfig);

	bool testmode=0;
	bool hidden=0;
	bool isvalidmove(Position pos);
	void displaydesk();
	void move(Position pos, int player);
	bool isgameover();
	int getlinesize();
	pair<int, int> getScores();


	//change to showdesk, getsize...!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Desk* providedesk();
	
};

