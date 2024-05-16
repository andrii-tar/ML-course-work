//#include <SFML/Graphics.hpp>
#include<Windows.h>
#include <iostream>
#include <stdlib.h>     
#include <time.h> 
#include<string>
#include<chrono>
#include<stdio.h>
#include<cstdio>
#include<string>

#include<thread>

#include"GameEnv.h"
#include"Players.h"

using namespace std;

int main()
{
    Config config;
    config.hidden = 0;
    GameLauncher mygame(config);

    HumanPlayer* pl1, * pl2;
    RandomPlayer* rp1, * rp2;
  //  AlgoPlayer* ap1, * ap2;

    AlgoPlayer* ap1 = new AlgoPlayer(1);
    

    pl1 = new HumanPlayer(1);
    pl2 = new HumanPlayer(2);

    rp1 = new RandomPlayer(1);
    rp2 = new RandomPlayer(2);

    auto scores = mygame.start(ap1, rp2);
   
    cout << scores.first << " " << scores.second << endl;
    cout << "hello\n";
    return 0;
};

//**************** add UI


