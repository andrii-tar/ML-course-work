#include "IPlayer.h"


IPlayer::IPlayer(int id)
{
	this->player_id = id;
}

IPlayer::~IPlayer()
{
}





void IPlayer::initValues(Desk* desk)
{
}

void IPlayer::updatePlayer(Position pos, Desk* desk)
{
}




PatternState IPlayer::getPatternState(State state)
{
	switch (state)
	{
	case Empty: {
		return PatternState::Free;
		break;
	}
	case PlayedNought: {
		return PatternState::Blocked;
		break;
	}
	case PlayedCross: {
		return PatternState::Blocked;
		break;
	}
	default:
	{
		if (state == this->player_id)
			return PatternState::My;
		else
			return PatternState::Enemy;
		break;
	}
	}

}
