#include "Desk.h"

void Desk::setstate(Position pos, State newstate)
{
	if(desk[pos.row][pos.col]==State::Empty)
	desk[pos.row][pos.col] = newstate;
}

Desk::Desk(Config config)
{
	this->height = config.getheight();
	this->width = config.getwidth();
	this->cross_line_cnt = 0;
	this->nought_line_cnt = 0;
	//this->testmode = config.testing;
	this->linesize = config.getlinesize();
	desk = vector<vector<State>>(this->height, vector<State>(this->width, State::Empty));
}

State Desk::getstate(int row, int col)
{
	return this->desk[row][col];
}

State Desk::getstate(Position pos)
{
	return this->getstate(pos.row, pos.col);
}

vector<vector<State>> Desk::showdesk()
{
	return this->desk;
}

void Desk::getdesksize(int& h, int& w)
{
	h = height;
	w = width;
}

int Desk::getheight()
{
	return this->height;
}

int Desk::getwidth()
{
	return this->width;
}

int Desk::getlinesize()
{
	return this->linesize;
}

bool Desk::isInsideDesk(Position position)
{
	if (position.row >= 0 && position.row < this->height
		&& position.col >= 0 && position.col < this->width)
		return true;
	return false;
}

void Desk::cross(Position pos)
{
	if (desk[pos.row][pos.col] > 3)
	{
		//log error
	}
	else
	{
		if (this->desk[pos.row][pos.col] == State::Cross)
			this->desk[pos.row][pos.col] = State::PlayedCross;
		else 
			this->desk[pos.row][pos.col] = State::PlayedNought;
	}
}
