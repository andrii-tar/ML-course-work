#pragma once
#include"GameEnv.h"
#include"Players.h"
#include"Eigen/Dense"
#include"ShortPatternSet.h"
using namespace Eigen;


class ShortGA :
    public IPlayer
{
    bool initialized = false;

    MatrixXd cellWeights;

    ShortPatternSet patternSet;


    double evaluateCell(Position position, Desk* desk);

    PatternState getPatternState(State state);
    double getPatternWeight(Pattern pattern);
public:
    void initValues(Desk* desk);
    ShortGA(int id, ShortPatternSet patternSet);
    Position makeMove(Desk* desk);


    void updatePlayer(Position position, Desk* desk);
};

