
#include "AlgoTools.h"

Limits calclimits(int row, int col, MainDirection dir, Desk* desk)
{
	Limits lim;
	int linesize = desk->getlinesize();
	int height, width;
	desk->getdesksize(height, width);
	switch (dir)
	{
	case Vertical:
	{
		lim.y1 = max(0, row - linesize + 1);
		lim.y2 = min(height - 1, row + linesize - 1);

		lim.x1 = lim.x2 = col;

		break;
	}
	case Horizontal:
	{
		lim.x1 = max(0, col - linesize + 1);
		lim.x2 = min(width - 1, col + linesize - 1);

		lim.y1 = lim.y2 = row;

		break;
	}
	case Diagonal_up:
	{

		lim.x1 = col - linesize + 1;
		lim.x2 = col + linesize - 1;

		lim.y1 = row + linesize - 1;
		lim.y2 = row - linesize + 1;

		int d;
		d = min(lim.x1, height - lim.y1 - 1);

		if (d < 0)
		{
			lim.y1 += d;
			lim.x1 -= d;
		}

		d = max(lim.x2 - width + 1, 0 - lim.y2);

		if (d > 0)
		{
			lim.y2 += d;
			lim.x2 -= d;
		}

		break;
	}
	case Diagonal_down:
	{
		lim.x1 = col - linesize + 1;
		lim.x2 = col + linesize - 1;

		lim.y1 = row - linesize + 1;
		lim.y2 = row + linesize - 1;

		int d;
		d = min(lim.x1, lim.y1);

		if (d < 0)
		{
			lim.y1 -= d;
			lim.x1 -= d;
		}

		d = max(lim.x2 - width + 1, lim.y2 - height + 1);

		if (d > 0)
		{
			lim.y2 -= d;
			lim.x2 -= d;
		}

		break;
	}
	}
	return lim;
}


/*

Position getMainDirectionDelta(MainDirection direction)
{
	switch (direction)
	{
	case MainDirection::Vertical:
		return Position(1, 0);
		break;
	case MainDirection::Horizontal:
		return Position(0, 1);
		break;
	case MainDirection::Diagonal_up:
		return Position(-1, 1);
		break;
	case MainDirection::Diagonal_down:
		return Position(1, 1);
		break;
	default:
		break;
	}
	return Position();
}

*/
