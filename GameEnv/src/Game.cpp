#include "Game.h"
using namespace std;



Game::Game(Config config)
{
	this->desk = new Desk(config);
	//this->height = config.getheight();
	//this->width = config.getwidth();
	this->linesize = config.getlinesize();
	this->firstwin = config.getfirstwin();
	this->cellcnt = desk->getheight() * desk->getwidth();
	this->testmode = config.testing;
	this->hidden = config.hidden;
	if (config.getMaxLinesNum() > 0)
		this->maxLinesNumber = config.getMaxLinesNum();
	//desk = vector<vector<Cell>>(this->height, vector<Cell>(this->width));
}

Desk* Game::providedesk()
{
	return this->desk;
}


bool Game::isvalidmove(Position pos)
{
	
	if ( pos.row < 0 || pos.col < 0
		|| pos.col >= desk->getwidth()
		|| pos.row >= desk->getheight() )
		return false;
	if(this->desk->getstate(pos.row, pos.col) != State::Empty)
		return false;

	return true;
}



void Game::displaydesk()
{
	if (this->hidden == 1)
		return;
	int i, j;
	cout << "\t\t-----------------------------------\n";
	cout << "Score\t Desk size: " << desk->getheight() << "*" << desk->getwidth() << "  ";
	cout << "Cross length: " << this->linesize << "\nPlayer1: " << p1score << "\tPlayer2 : " << p2score << "\n\t";
	
	for (i = 0; i < desk->getwidth(); i++)
		if ((i+1) % 5 == 0) cout << "|";
		else cout << " ";
	cout << "\n\t";
	for (i = 0; i < desk->getwidth(); i++)
		cout << "-";
	cout << "\n";


	for (i = 0; i < desk->getheight(); i++)
	{
		cout << i+1<<" | \t";
		for (j = 0; j < desk->getwidth(); j++)
		{

			switch (this->desk->getstate(i, j))
			{
			case State::Empty:
			{
				cout << ".";
				break;
			}
			case State::Nought: {
				cout << "O";
				break;
			}
			case State::Cross: {
				cout << "X";
				break;
			}
			case State::PlayedNought: {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_INTENSITY | 
					FOREGROUND_RED |
					// FOREGROUND_GREEN |
					FOREGROUND_BLUE);
				cout << "o";
				break;
			}
			case State::PlayedCross: {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_INTENSITY | 
					//FOREGROUND_RED |
					FOREGROUND_GREEN | 
					FOREGROUND_BLUE
				);
				cout << "x";
				break;
			}
			default:
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
					FOREGROUND_INTENSITY |
					FOREGROUND_RED //|
					//FOREGROUND_GREEN |
					//FOREGROUND_BLUE
				);
				cout << this->desk->getstate(i, j);
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_INTENSITY |
				FOREGROUND_RED |
				FOREGROUND_GREEN 
				| FOREGROUND_BLUE
			);
		}
		cout << "\t\t";

		//for (j = 0; j < desk->getwidth(); j++) cout << this->desk[i][j].;
		cout << endl;
	}

	cout << "\n\n";
	

	/*
	for (i = 0; i < logs.size(); i++)
	{
		cout << i << ")  " << logs[i][0] << " " << logs[i][1] << " - ";
		switch (logs[i][2])
		{
		case 1:
		{
			cout << "vert";
			break;
		}
		case 2:
		{
			cout << "hor";
			break;
		}
		case 3:
		{
			cout << "down";
			break;
		}
		case 4:
		{
			cout << "up";
			break;
		}
		default:
			break;
		}

		cout << "  "<<logs[i][3]<<endl;
	}
	*/
}



void Game::update(int row, int col)
{
	int v = 0, h = 0, u = 0, d = 0, t;
	int x, y;

	//cout << "\nGame::update: begin\t";
	//cout << row << " " << col << endl;

	t = this->desk->getstate(row, col);

	x = col;
	y = row;
	v = checkany(&y, &x, MainDirection::Vertical);
	//cout << "v: " << v << endl;
	if (v)
	{

		if (this->desk->getstate(y, x) > 2)
			logs.push_back({ x, y, 1, this->desk->getstate(y, x) });
		else
			crossvert(y, x);
		//logs.push_back({ x, y, 1, t});
		if (t == 1) p1score++;
		else p2score++;
		return;
	}

	x = col;
	y = row;
	h = checkany(&y, &x, MainDirection::Horizontal);
	//cout << "h: " << h << endl;
	if (h)
	{
		//cout << "vert: " << v << "  hor: " << h << "  down: " << d << "  up: " << u << endl;

		if (this->desk->getstate(y, x) > 2)
			logs.push_back({ x, y, 2, this->desk->getstate(y, x) });
		else
			crosshor(y, x);
		//logs.push_back({ x, y, 2, t });
		if (t == 1) p1score++;
		else p2score++;
		return;
	}

	x = col;
	y = row;
	d = checkany(&y, &x, MainDirection::Diagonal_down);
	//cout << "dd: " << d << endl;
	if (d)
	{
		//cout << "vert: " << v << "  hor: " << h << "  down: " << d << "  up: " << u << endl;

		if (this->desk->getstate(y, x) > 2)
			logs.push_back({ x, y, 2, this->desk->getstate(y, x) });
		else
			crossdiag_down(y, x);
		//logs.push_back({ x, y, 3, t });

		if (t == 1) p1score++;
		else p2score++;
		return;
	}


	x = col;
	y = row;
	u = checkany(&y, &x, MainDirection::Diagonal_up);
	//cout << "du: " << u << endl;
	//cout << "check42\t";
	if (u)
	{
		//cout << "vert: " << v << "  hor: " << h << "  down: " << d << "  up: " << u << endl;

		if (this->desk->getstate(y, x) > 2)
			logs.push_back({ x, y, 2, this->desk->getstate(y, x) });
		else
			crossdiag_up(y, x);
		//logs.push_back({ x, y, 4, t });
		if (t == 1) p1score++;
		else p2score++;
		return;
	}


	//cout << "check5\t";

	/*
	if (v || h || d || u)
	{
		if (t == 1) p1score++;
		else p2score++;
	}
	*/
	//cout << "vert: " << v << "  hor: " << h << "  down: " << d << "  up: " << u << endl;
}


Limits Game::calclimits2(int row, int col, MainDirection dir)
{
	Limits lim;

	switch (dir)
	{
	case Vertical:
	{
		lim.y1 = max(0, row - (*this).linesize + 1);
		lim.y2 = min(desk->getheight() - 1, row + (*this).linesize - 1);

		lim.x1 = lim.x2 = col;

		break;
	}
	case Horizontal:
	{
		lim.x1 = max(0, col - (*this).linesize + 1);
		lim.x2 = min(desk->getwidth() - 1, col + (*this).linesize - 1);

		lim.y1 = lim.y2 = row;

		break;
	}
	case Diagonal_up:
	{

		lim.x1 = col - (*this).linesize + 1;
		lim.x2 = col + (*this).linesize - 1;

		lim.y1 = row + (*this).linesize - 1;
		lim.y2 = row - (*this).linesize + 1;

		int d;
		d = min(lim.x1, desk->getheight() - lim.y1 - 1);

		if (d < 0)
		{
			lim.y1 += d;
			lim.x1 -= d;
		}

		d = max(lim.x2 - desk->getwidth() + 1, 0 - lim.y2);

		if (d > 0)
		{
			lim.y2 += d;
			lim.x2 -= d;
		}

		break;
	}
	case Diagonal_down:
	{
		lim.x1 = col - (*this).linesize + 1;
		lim.x2 = col + (*this).linesize - 1;

		lim.y1 = row - (*this).linesize + 1;
		lim.y2 = row + (*this).linesize - 1;

		int d;
		d = min(lim.x1, lim.y1);

		if (d < 0)
		{
			lim.y1 -= d;
			lim.x1 -= d;
		}

		d = max(lim.x2 - desk->getwidth() + 1, lim.y2 - desk->getheight() + 1);

		if (d > 0)
		{
			lim.y2 -= d;
			lim.x2 -= d;
		}

		break;
	}
	}
	//cout << "Game::Calclimits:   x:" << lim.x1 << "->" << lim.x2;
	//cout << "  y:" << lim.y1 << "->" << lim.y2 << endl << endl;
	return lim;
}



int Game::checkany(int* row, int* col, MainDirection dir)
{
//	cout << "\ncheck any : " << *row << " " << *col << " " << dir << endl;
	int x = *col, y = *row;
	int i, j, dx, dy;

	Limits lim = calclimits(*row, *col, dir, this->desk);
	dx = ((lim.x2 - lim.x1) / max(abs(lim.x2 - lim.x1), 1));
	dy = ((lim.y2 - lim.y1) / max(abs(lim.y2 - lim.y1), 1));

	int k = max(abs(lim.x2 - lim.x1), abs(lim.y1 - lim.y2))+1;
//	cout << "k= " << k << endl;
	if (lim.x1 == lim.x2 && lim.y1 == lim.y2) dx = dy = 1;

	int cnt = 0, mcnt = 0;
	int t = this->desk->getstate(*row, *col);
	//cout << "state: " << t << "\t\t"<<dy<<" "<<dx<<endl;

	//for (i = lim.y1, j = lim.x1; i != lim.y2 || j != lim.x2; i += dy, j += dx)
	
	i = lim.y1;
	j = lim.x1;
	for (int step=0;step<k;step++)
	{
		
	//	cout << "\ty " << i << " x " << j << " "<<this->desk->getstate(i, j) << endl;
		if (this->desk->getstate(i, j) == t)
		{
			if (cnt == 0)
			{
				*col = j;
				*row = i;
			}
			cnt++;
		mcnt = max(cnt, mcnt);
		}
		else if (cnt < this->linesize) cnt = 0;
		i += dy;
		j += dx;
	}

	//cout << "limits: " << lim.y1 << " " << lim.x1 << " " << lim.y2 << " " << lim.x2 << endl;
	//cout << mcnt << endl;
	if (mcnt >= this->linesize)
	{
		return 1;
	}
	return 0;
}




void Game::crossvert(int row, int col)
{
	//if(this->testmode) cout << "\n\tcrossing vert\n";

	for (int i = 0; i < (*this).linesize; i++)
		this->desk->cross(Position(row + i,col));
}

void Game::crosshor(int row, int col)
{
	//if (this->testmode) cout << "\n\tcrossing hor\n";

	for (int i = 0; i < (*this).linesize; i++)
		this->desk->cross(Position(row,col + i));
}

void Game::crossdiag_down(int row, int col)
{
	//if (this->testmode) cout << "\n\tcrossing diag down\n";

	for (int i = 0; i < (*this).linesize; i++)
		this->desk->cross(Position(row+i, col + i));

}

void Game::crossdiag_up(int row, int col)
{
	//if (this->testmode) cout << "\n\tcrossing diag up\n";

	for (int i = 0; i < (*this).linesize; i++)
		this->desk->cross(Position(row-i, col + i));
}



void Game::move(Position pos, int player)
{
	//cout << "Game::move: " << row << " col=" << col<<"\t";
	if(this->desk->getstate(pos.row, pos.col) == State::Empty)
		this->desk->setstate(pos, player==1 ? State::Nought : State::Cross);

	this->update(pos.row, pos.col);
	this->cellcnt--;
}

bool Game::isgameover()
{
	if(this->usergameover)
		return true;

	if (this->firstwin && (p1score + p2score) > 0)
		return true;
	if (this->cellcnt == 0)
		return true;
	if (p1score >= this->maxLinesNumber ||  p2score >= this->maxLinesNumber)
		return true;

	return false;
}

int Game::getlinesize()
{
	return this->linesize;
}

pair<int, int> Game::getScores()
{
	return pair<int, int>(this->p1score, this->p2score);
}



















 


/*
int Game::getstate(int* row, int* col)
{
	return this->desk[y][x].getstate();
}
*/



/*
void Game::getdesksize(int* h, int* w)
{
	*h = this->height;
	*w = this->width;
}
*/

//old check funcs
/*
int Game::checkvert(int* row, int* col)
{
	int x = *col, y = *row;

	int i, i1, i2;
	int cnt = 0, mcnt = 0;
	int t = this->desk->getstate(*row, *col);

	i1 = max(0, y - (*this).linesize + 1);
	i2 = min(desk->getheight(), y + (*this).linesize);

	for (i = i1; i < i2; i++)
	{
		mcnt = max(cnt, mcnt);
		if (this->desk->getstate(i, x) == t)
		{
			if (cnt == 0)
			{
				*col = x;
				*row = i;
			}
			cnt++;
		}
		else if (mcnt < this->linesize) mcnt = cnt = 0;
	}

	if (mcnt >= this->linesize)
	{
		//cout << "cross v " << x1 << " " << y1 << endl;
		//this->crossvert(x1, y1);
		return 1;
	}
	return 0;
}

int Game::checkhor(int* row, int* col)
{
	int x = *col, y = *row;

	int  i, i1, i2;
	int cnt = 0, mcnt = 0;
	int t = this->desk->getstate(*row, *col);

	i1 = max(0, x - (*this).linesize + 1);
	i2 = min(desk->getwidth(), x + (*this).linesize);

	for (i = i1; i < i2; i++)
	{
		if (cnt > mcnt) mcnt = cnt;
		if (this->desk->getstate(y, i) == t)
		{
			if (cnt == 0)
			{
				*col = i;
				*row = y;
			}
			cnt++;
		}
		else if (mcnt < this->linesize) mcnt = cnt = 0;
	}

	if (mcnt >= this->linesize)
	{
		//cout << "cross h " << x1 << " " << y1 << endl;
		//this->crosshor(x1, y1);
		return 1;
	}
	return 0;

}

int Game::checkdiag_down(int* row, int* col)
{
	int x = *x1, y = *y1;
	int  i, i1, i2, j, j1, j2;
	int cnt = 0, mcnt = 0;
	int t = this->desk->getstate(*row, *col);

	i1 = max(0, x - (*this).linesize + 1);
	i2 = min((*this).width, x + (*this).linesize);

	j1 = max(0, y - (*this).linesize + 1);
	j2 = min((*this).height, y + (*this).linesize);

	for (i = i1, j = j1; i < i2 && j < j2; i++, j++)
	{
		if (cnt > mcnt) mcnt = cnt;

		if (this->desk[j][i].getstate() == t)
		{
			if (cnt == 0)
			{
				*x1 = i;
				*y1 = j;
			}
			cnt++;
		}
		else if (mcnt < this->linesize) cnt = 0;
	}

	if (mcnt >= this->linesize)
	{
		//cout << "cross d down " << x1 << " " << y1 << endl;
		//this->crossdiag_down(x1, y1);
		return 1;
	}
	return 0;

}

int Game::checkdiag_up(int* row, int* col)
{
	int x = *x1, y = *y1;
	int i, i1, i2, j, j1, j2;
	int cnt = 0, mcnt = 0;
	int t = this->desk->getstate(*row, *col);

	i1 = max(0, x - (*this).linesize + 1);
	i2 = min((*this).width, x + (*this).linesize);

	j1 = max(0, y - (*this).linesize + 1);
	j2 = min((*this).height, y + (*this).linesize);
	swap(j1, j2);

	for (i = i1, j = j1; i < i2 && j < j2; i++, j--)
	{
		if (cnt > mcnt) mcnt = cnt;

		if (this->desk[j][i].getstate() == t)
		{
			if (cnt == 0)
			{
				*x1 = i;
				*y1 = j;
			}
			cnt++;
		}
		else if (mcnt < this->linesize) cnt = 0;
	}

	if (mcnt >= this->linesize)
	{
		//cout << "cross d up " << x1 << " " << y1 << endl;
		//this->crossdiag_up(x1, y1);
		return 1;
	}
	return 0;

}
*/






//old update func
/*


void Game::update(int row, int col)
{
	int v = 0, h = 0, u = 0, d = 0, t;
	int x, y;
	t = this->desk->getstate(row, col);


	x = col;
	y = row;
	v = checkvert(&y, &x);
	if (v)
	{
		if (this->desk->getstate(y, x) > 2)
			logs.push_back({ x, y, 1, this->desk->getstate(y, x) });
		else
			crossvert(y, x);
		//logs.push_back({ x, y, 1, t});
		if (t == 1) p1score++;
		else p2score++;
		return;
	}


	x = col;
	y = row;
	h = checkhor(&y, &x);
	if (h)
	{
		if (this->desk->getstate(y, x) > 2)
			logs.push_back({ x, y, 2, this->desk->getstate(y, x) });
		else
			crosshor(y, x);
		//logs.push_back({ x, y, 2, t });
		if (t == 1) p1score++;
		else p2score++;
		return;
	}

	x = col;
	y = row;
	d = checkdiag_down(&y, &x);
	if (d)
	{
		if (this->desk->getstate(y, x) > 2)
			logs.push_back({ x, y, 2, this->desk->getstate(y, x) });
		else
			crossdiag_down(y, x);
		//logs.push_back({ x, y, 3, t });

		if (t == 1) p1score++;
		else p2score++;
		return;
	}

	x = col;
	y = row;
	u = checkdiag_up(&y, &x);
	if (u)
	{
		if (this->desk->getstate(y, x) > 2)
			logs.push_back({ x, y, 2, this->desk->getstate(y, x) });
		else

			crossdiag_up(y, x);
		//logs.push_back({ x, y, 4, t });
		if (t == 1) p1score++;
		else p2score++;
		return;
	}




cout << "vert: " << v << "  hor: " << h << "  down: " << d << "  up: " << u << endl;
}


*/
