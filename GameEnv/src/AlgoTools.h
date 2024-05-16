#pragma once

//#include"GameEnv.h"
#include<vector>
#include"Desk.h"
using namespace std;



enum MainDirection : int
{
	Vertical = 0,
	Horizontal,
	Diagonal_up,
	Diagonal_down
};


static const vector<MainDirection> ALL_MAIN_DIRECTIONS = {
	MainDirection::Vertical,
	MainDirection::Horizontal,
	MainDirection::Diagonal_up,
	MainDirection::Diagonal_down
};

enum Check_direction :int
{
	Left = 0,
	Up_left,
	Up,
	Up_right,
	Right,
	Down_right,
	Down,
	Down_left
};


struct Mask_values
{
	struct MyStruct
	{
		float free = 0, blocked = 0;
	};
	vector<MyStruct> my, enemy;
	Mask_values(int linesize);
	Mask_values();
};

static const vector<Check_direction> check_dir
{
	Left, Up_left, Up, Up_right,
	Right, Down_right, Down, Down_left
};
struct Limits
{
	int x1, x2, y1, y2;
};



Limits calclimits(int row, int col, MainDirection dir, Desk* desk);

