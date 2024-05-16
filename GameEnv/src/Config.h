#pragma once

struct Position
{
	int row, col;
	Position()
	{
	}
	Position(int row, int col)
	{
		this->row = row;
		this->col = col;
	}
	bool operator == (const Position& p)
	{
		return row == p.row && col == p.col;
	}

	bool operator != (const Position& p)
	{
		return row != p.row || col != p.col;
	}
};

class Config
{
private:
	int height = 20;
	int width = 35;
	int linesize = 5;
	bool firstwin = 0;
	int maxLines = 25;
	
public:
	int getheight();
	int getwidth();
	int getlinesize();
	bool getfirstwin();

	int getMaxLinesNum();

	bool testing = 0;
	bool hidden = 1;
	Config(int newheight, int newwidth, int newMaxLines, int newlinesize);
	Config();
};




