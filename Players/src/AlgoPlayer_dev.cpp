#include"AlgoPlayer.h"


Mask_values::Mask_values(int linesize)
{
	//cout << "maskval init\n";
	my = *(new vector<MyStruct>(linesize - 1));
	enemy = *(new vector<MyStruct>(linesize - 1));


	int val = (linesize - 1) * 4;

	linesize--;

	my[linesize - 1].blocked = val--;		//16
	enemy[linesize - 1].blocked = val--;	//15

	my[linesize - 2].free = val--;			//14
	enemy[linesize - 1].free = val--;		//13

	enemy[linesize - 2].blocked = 1;		//1

	if (linesize > 3) my[linesize - 3].free = val--;	//12
	my[linesize - 2].blocked = val--;		//11

	my[linesize - 1].free = val--;			//10
	enemy[linesize - 2].free = val--;		//9

	for (int i = linesize - 1; i >= 0; i--)
	{
		if (my[i].free == 0) my[i].free = val--;
	}

	for (int i = linesize - 1; i >= 0; i--)
	{
		if (my[i].blocked == 0) my[i].blocked = val--;
	}

	for (int i = linesize - 1; i >= 0; i--)
	{
		if (enemy[i].free == 0) enemy[i].free = val--;
	}

	for (int i = linesize - 1; i >= 0; i--)
	{
		if (enemy[i].blocked == 0) enemy[i].blocked = val--;
	}

	/*
	cout << "\nfree:  ";
	for (auto t : my) cout << t.free << " ";
	cout << "\nblocked:  ";
	for (auto t : my) cout << t.blocked << " ";
	cout << "\nfree:  ";
	for (auto t : enemy) cout << t.free << " ";
	cout << "\nblocked:  ";
	for (auto t : enemy) cout << t.blocked << " ";
	cout << "inited\n";
	Sleep(2000);
	*/
}

Mask_values::Mask_values()
{}



Limits AlgoPlayer::calcLimitsInverse(Position pos, Desk* desk, Check_direction dir)
{
	Limits lim;
	int w, h;
	int linesize = desk->getlinesize();
	desk->getdesksize(h, w);

	//provided that direction is awailable
	switch (dir)
	{
	case Left:
	{
		lim.y1 = lim.y2 = pos.row;
		lim.x1 = pos.col - 1;
		lim.x2 = max(pos.col - (linesize - 1), 0);
		break;
	}
	case Up_left:
	{
		lim.x1 = pos.col - 1;
		lim.x2 = pos.col - linesize + 1;

		lim.y1 = pos.row - 1;
		lim.y2 = pos.row - linesize + 1;

		int d = min(lim.x1, lim.y1);

		if (d < 0)
		{
			lim.y2 -= d;
			lim.x2 -= d;
		}
		break;
	}
	case Up:
	{
		lim.y1 = pos.row - 1;
		lim.y2 = max(0, pos.row - (linesize - 1));
		lim.x1 = lim.x2 = pos.col;
		break;
	}
	case Up_right:
	{
		lim.x1 = pos.col + 1;
		lim.x2 = pos.col + linesize - 1;

		lim.y1 = pos.row - 1;
		lim.y2 = pos.row - linesize + 1;

		int d = max(lim.x2 - w + 1, 0 - lim.y2);
		if (d > 0)
		{
			lim.y2 -= d;
			lim.x2 -= d;
		}
		break;
	}
	case Right:
	{
		lim.y1 = lim.y2 = pos.row;
		lim.x1 = pos.col + 1;
		lim.x2 = min(pos.col + (linesize - 1), w-1);
		break;
	}
	case Down_right:
	{
		lim.x1 = pos.col + 1;
		lim.x2 = pos.col + linesize - 1;

		lim.y1 = pos.row  + 1;
		lim.y2 = pos.row + linesize - 1;

		int d = max(lim.x2 - w + 1, lim.y2 - h + 1);

		if (d > 0)
		{
			lim.y2 -= d;
			lim.x2 -= d;
		}
		break;
	}
	case Down:
	{
		lim.y1 = pos.row + 1;
		lim.y2 = min(h-1, pos.row + (linesize - 1));
		lim.x1 = lim.x2 = pos.col;
		break;
	}
	case Down_left:
	{
		lim.x1 = pos.col - 1;
		lim.x2 = pos.col - linesize + 1;

		lim.y1 = pos.row + 1;
		lim.y2 = pos.row + linesize - 1;

		int d = max(0 - lim.x2, lim.y2 - h + 1);
		if (d > 0)
		{
			lim.y2 -= d;
			lim.x2 -= d;
		}
		break;
	}
	default:
		break;
	}
	return lim;
}

void AlgoPlayer::updatePlayer(Position pos, Desk* desk)
{
	//cout << "player "<<this->player_id<<" update start\n";
	int linesize = desk->getlinesize();

	int i, j, i1, i2, j1, j2;
	int h, w;
	desk->getdesksize(h, w);
	i1 = max(0, pos.col - linesize + 1);
	i2 = min(w, pos.col + linesize);

	j1 = max(0, pos.row - linesize + 1);
	j2 = min(h, pos.row + linesize);

	for (j = j1; j < j2; j++)
		for (i = i1; i < i2; i++)
		{
			Position p(j, i);
			this->updatecell(p, desk);
		}

	int t = this->vcell[pos.row][pos.col].prindex;
	//cout << "test  "<<t<<"\n";
	if (t < vprio.size())
	{
		swap(this->vprio[t], this->vprio.back());
		vprio.pop_back();
	}
	//cout << "tested\n";
	//cout << "\n\n\n";
	//this->distances();
	//cout << "\n\n\n";
	
	//cout << "player_update finish\n";
	//Sleep(1000);

}


double AlgoPlayer::upd_any_direction(Position pos, Desk* desk, Check_direction dir)
{


	//cout << "start upd any dir\n";
	int linesize = desk->getlinesize();
	int x = pos.col, y = pos.row;
	int i, j, dx, dy;
	double q = 0;
	int h, w;
	Limits lim;
	desk->getdesksize(h, w);


	switch (dir)
	{
	case Left:
		if (pos.col == 0) return 0;
		break;
	case Up_left:
		if (pos.col == 0 || pos.row == 0) return 0;
		break;
	case Up:
		if (pos.row == 0) return 0;
		break;
	case Up_right:
		if (pos.col == w - 1 || pos.row == 0) return 0;
		break;
	case Right:
		if (pos.col == w - 1) return 0;
		break;
	case Down_right:
		if (pos.col == w - 1 || pos.row == h - 1) return 0;
		break;
	case Down:
		if (pos.row == h - 1) return 0;
		break;
	case Down_left:
		if (pos.col == 0 || pos.row == h - 1) return 0;
		break;
	}

	lim = calcLimitsInverse(pos, desk, dir);

	//cout << "\tupd test\n";

	

	dx = ((lim.x2 - lim.x1) / max(abs(lim.x2 - lim.x1), 1));
	dy = ((lim.y2 - lim.y1) / max(abs(lim.y2 - lim.y1), 1));

	if (lim.x1 == lim.x2 && lim.y1 == lim.y2) dx = dy = 1;


	int lcnt = 0; 
	bool blocked = 0;
	State st, target, mystate;
	target = desk->getstate(Position(lim.y1, lim.x1));

	
	if (target == State::PlayedCross || 
		target == State::PlayedNought
		) return 0;
		
	mystate = desk->getstate(pos);

	//cout << "\t\tupd test 2     " << dir << "\n";
	//cout << lim.y1 << " y " << lim.y2 << "\t" << lim.x1 << " x " << lim.x2 << endl;

	for (i = lim.y1, j = lim.x1; i <= lim.y2 && j <= lim.x2; i += dy, j += dx)
	{
		st = desk->getstate(Position(i, j));
		if (st == target)
			lcnt++;
		else break;
	}

	//cout << "\t\t\tupd test 3\n";

	if (target == State::Empty) return linesize / 2;//-1;// lcnt;

	blocked = isblocked(pos, desk, dir, lcnt);

	//cout << "\t\t\t\tupd test 4   " << lcnt << "  " << mask_values.my.size() << "  " << mask_values.enemy.size() <<  endl;


	if (lcnt == 0) return 0;

	if (!blocked)
		if (target == mystate)
			q = mask_values.my[lcnt-1].free;
		else
			q = mask_values.enemy[lcnt-1].free;
	else
		if (target == mystate)
			q = mask_values.my[lcnt-1].blocked;
		else
			q = mask_values.enemy[lcnt-1].blocked;
			
	//cout << "\t\t\t\t\tupd test 5\n";

	//if (target != mystate)
	{
	//	cout << target << "  " << mystate << endl;
		//cout << "\t\t\t\tENEMY  " << q << "\n";
		//Sleep(20);
	}
	//cout << "finish upd any dir   "<<"\n";

	//this->vcell[pos.row][pos.col].val += q;
	return q;

}

bool AlgoPlayer::isblocked(Position pos, Desk* desk, Check_direction dir, int line)
{
	int h, w;
	desk->getdesksize(h, w);
	//cout << "algo blocked   "<<pos.row<<" "<<pos.col<<" "<<line<<"\n";
	State emp = State::Empty;
	switch (dir)
	{
	case Left:
		if (pos.col - line <= 0) return true;
		if (desk->getstate(Position(pos.row, pos.col - line - 1)) != emp)
			return true;
		break;
	case Up_left:
		if (pos.col - line <= 0 || pos.row - line <= 0) return true;
		if (desk->getstate(Position(pos.row - line - 1, pos.col - line - 1)) != emp)
			return true;
		break;
	case Up:
		if (pos.row -line <= 0) return true;
		if (desk->getstate(Position(pos.row - line - 1, pos.col)) != emp)
			return true;
		break;
	case Up_right:
		if (pos.col + line >= w - 1 || pos.row -line <= 0) return true;
		if (desk->getstate(Position(pos.row - line -1, pos.col + line + 1)) != emp)
			return true;
		break;
	case Right:
		if (pos.col + line >= w - 1) return true;
		if (desk->getstate(Position(pos.row, pos.col + line + 1)) != emp)
			return true;
		break;
	case Down_right:
		if (pos.col + line >= w - 1 || pos.row + line >= h - 1) return true;
		if (desk->getstate(Position(pos.row+line +1, pos.col + line + 1)) != emp)
			return true;
		break;
	case Down:
		if (pos.row+line >= h - 1) return true;
		if (desk->getstate(Position(pos.row + line + 1, pos.col)) != emp)
			return true;
		break;
	case Down_left:
		if (pos.col -line  <= 0 || pos.row+line >= h - 1) return true;
		if (desk->getstate(Position(pos.row + line + 1, pos.col - line - 1)) != emp)
			return true;
		break;
	}
	return false;
}


void AlgoPlayer::updatecell(Position pos, Desk* desk)
{
	//cout << "\n\nalgo update cell  " << pos.row << " " << pos.col << "  " << (State)desk->getstate(pos) << "\n";

	//cout << "Old value:\t" << vcell[pos.row][pos.col].val << endl;
	this->vcell[pos.row][pos.col].val = 0;// this->vcell[pos.row][pos.col].dist;

	double q1 = 0, q2 = 0;

	q1 = this->upd_around(pos, desk);

	this->vcell[pos.row][pos.col].val += q1;
	//cout << "Mid value:\t" << vcell[pos.row][pos.col].val;// << endl;

	for (auto dir : check_dir)
	{
		//q2+= this->upd_any_direction(pos, desk, dir);
		q2 = max(q2, this->upd_any_direction(pos, desk, dir));
	}
	//cout << "\t\tQ2 = " << q2 << endl;
	this->vcell[pos.row][pos.col].val += q2;

	//cout << "New value:\t" << vcell[pos.row][pos.col].val << endl;
	//Sleep(500);

}













AlgoPlayer::AlgoPlayer(int id)
	:IPlayer(id)
{
}

void AlgoPlayer::initValues(Desk* desk)
{
	int linesize = desk->getlinesize();

	if (!initiated)
	{
		this->mask_values = *(new Mask_values(desk->getlinesize()));

		linesize = desk->getlinesize();
		defcellval = 2.0 / (linesize * linesize * 1.0);

		int h, w;
		desk->getdesksize(h, w);
		this->vcell = vector<vector<AdvancedCell>>(h, vector<AdvancedCell>(w));

		int i, j;

		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				vcell[i][j].val = 0;

				if (i == h / 2 && j == w / 2) vcell[i][j].dist = 1;
				else vcell[i][j].dist = 1.0 / (sqrt(
					pow((h / 2 - i), 2) + pow((w / 2 - j), 2)
				) * 1.0);
				vcell[i][j].val = 0;
				vcell[i][j].x = j;
				vcell[i][j].y = i;
				vcell[i][j].prindex = vprio.size();
				vprio.push_back(&vcell[i][j]);
				updatecell(Position(i, j), desk);
			}
		}

		initiated = 1;
	}
}

/*
void AlgoPlayer::updatecell(Position pos, Desk* desk)
{
	int i, j, i1, i2, j1, j2;
	int linesize =desk->getlinesize();
	double q1 = 0, q2 = 0;


	i1 = max(0, pos.col - linesize + 1);
	i2 = min((*this).w, pos.col + linesize);

	j1 = max(0, pos.row - linesize + 1);
	j2 = min((*this).h, pos.row + linesize);

	this->vcell[pos.row][pos.col].val = 0;

	this->upd_around(pos, desk);


	for (j = 1; j < linesize && min(pos.col, pos.row)-j>=0; j++)
	{

		int t = desk->getstate(pos.col - j, pos.row - j);
		if (t != this->player_id && t != 0) j = linesize;
		if (t == this->player_id)
			q2 += (linesize - j) * this->defcellval;
	}
	this->vcell[pos.row][pos.col].val += q1 + q2;



}
*/


double AlgoPlayer::upd_around(Position pos, Desk* desk)
{
	int linesize = desk->getlinesize();

	int i, j, i1, i2, j1, j2;

	double q1 = 0;

	int h, w;
	desk->getdesksize(h, w);

	i1 = max(0, pos.col - linesize + 1);
	i2 = min(w, pos.col + linesize);

	j1 = max(0, pos.row - linesize + 1);
	j2 = min(h, pos.row + linesize);

	for (j = j1; j < j2; j++)
		for (i = i1; i < i2; i++)

		{
			if (desk->getstate(j, i) == desk->getstate(pos))
			{
				int d = max(abs(pos.row - j), abs(pos.col - i));
				if (abs(pos.row - j) == 0 ||
					abs(pos.col - i) == 0 ||
					abs(pos.row - j) == abs(pos.col - i))
				{
					q1 += 1.0 / (1.0 * (d + 1));
				}
				else
					q1 += 1.0 / ((d + 1) * 2.0);

			}
		}
	return q1;
}



Position AlgoPlayer::makeMove(Desk* desk)//Game* game)
{
	Position pos;

	this->initValues(desk);

	sort(this->vprio.begin(), this->vprio.end(), &compfunc);


	for (int i = 0; i < vprio.size(); i++)
		vprio[i]->prindex = i;

	int i = vprio.size() - 1;


	int x, y;
	x = this->vprio[i]->x;
	y = this->vprio[i]->y;

	swap(vprio[i], vprio.back());
	vprio.pop_back();

	pos.col = x;
	pos.row = y;

	return pos;
}