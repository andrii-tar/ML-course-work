#pragma once
#include"GameEnv.h"


class AlgoPlayer :public IPlayer
{
private:
	double defcellval;

	bool initiated = 0;

	struct AdvancedCell
	{
		int x, y, prindex;
		double val, dist;
	};

	vector<AdvancedCell*>vprio;
	vector<vector<AdvancedCell>>vcell;

	static bool compfunc(AdvancedCell* c1, AdvancedCell* c2)
	{
		if (c1->val != c2->val)
			return c1->val < c2->val;
		return c1->dist < c2->dist;
	}



	Mask_values mask_values;
	


	void initValues(Desk* desk);
	void updatecell(Position pos, Desk* desk);

	double upd_around(Position pos, Desk* desk);

	double upd_any_direction(Position pos, Desk* desk, Check_direction dir);
	bool isblocked(Position pos, Desk* desk, Check_direction dir, int line);
	Limits calcLimitsInverse(Position pos, Desk* desk, Check_direction dir);

public:
	AlgoPlayer(int id);
	Position makeMove(Desk* desk);

	void updatePlayer(Position pos, Desk* desk);

	void distances();
};

