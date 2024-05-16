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
#include<vector>
#include<thread>

#include"GameEnv.h"
#include"Players.h"
#include"LineNN.h"
#include"SquareNN.h"
#include"StarNN.h"
using namespace std;

//D:\Projects\libraries\cv-plot\CvPlot\inc

/*
#define CVPLOT_HEADER_ONLY 
#include "CvPlot/cvplot.h"*/

int main()
{
    Config config;// = Config(20, 35, 25, 5);
    config.hidden = 0;


    HumanPlayer* pl1, * pl2;
    RandomPlayer* rp1, * rp2;
    AlgoPlayer* ap1, * ap2;


    pl1 = new HumanPlayer(1);
    pl2 = new HumanPlayer(2);

    rp1 = new RandomPlayer(1);

    vector<int> scrp1, scrp2;
    vector<int> xAxis;

    vector<ShortPatternSet> currentGen(25);

    currentGen = ShortPatternSet::loadPatternSets(25, config.getlinesize());
   //StarNN* reload = new StarNN(1, false, "star_nn_10_v1.dnn", false);
  // reload = new StarNN(1, false, "star_nn_10_v2.dnn", false);
   // reload = new StarNN(1, false, "star_nn_10_v2.dnn", false);
   // LineNN* line_reload = new LineNN(1, false, "line_nn_10_v1.dnn", false);
    //line_reload = new LineNN(2, false, "line_nn_10_v2.dnn", false);

    int numEpochs = 1000;
    if (config.hidden == 0)
        numEpochs = 1;
    for (int i = 0; i < numEpochs; i++)
    {
        int playerNum = 1;
        if (i % 10 > 5)
            playerNum = 2;

        rp1 = new RandomPlayer(1);
        rp2 = new RandomPlayer(2);
        GameLauncher mygame(config);
        ShortGA* sga2 = new ShortGA(2, currentGen[0]);
        ShortGA* sga1 = new ShortGA(1, currentGen[0]);

        ap1 = new AlgoPlayer(1);
        ap2 = new AlgoPlayer(3-playerNum);
        //LineNN* lnn1 = new LineNN(1, true, "line_nn_10_v1.dnn", true);
       // LineNN* lnn2 = new LineNN(2, false, "line_nn_10_v1.dnn", true);
        //StarNN* starNN1 = new StarNN(playerNum, true, "star_nn_10_v1.dnn", true);
        StarNN* starNN1 = new StarNN(1, false , "ok_star_nn_10_v1.dnn", true);
        StarNN* starNN2 = new StarNN(2, false, "ok_star_nn_10_v1.dnn", true);
         //SquareNN* sqNN2 = new SquareNN(2, false, "square_nn_10.dnn", true);
         //  LineNN* lnn2 = new LineNN(2, true, true, "line_nn_10_v2.dnn");
        
        auto scores = mygame.start(ap1, ap2);
        //auto scores = mygame.start(starNN1, ap2);
        cout << scores.first << " " << scores.second;
        if (scores.first >= scores.second)
            cout << "\t-";
        cout << endl;

        if (i % 5 == 0)
        { //rp2 = new RandomPlayer(2);
            GameLauncher test_game(config);
            // ShortGA* sga2 = new ShortGA(2, currentGen[0]);
           // scores = test_game.start(starNN1, ap2);

            cout << "\t" << i << " / " << numEpochs << endl;
           //writeVectorToCSV({ i, scores.first, scores.second }, NN_LEARNING_SCORES);
           // if (i % 20 == 0)
           //     starNN1->saveNetwork(to_string(i) + "_iter_star_nn_10_v1.dnn");

            scrp1.push_back(scores.first);
            scrp2.push_back(scores.second);
            xAxis.push_back(i);
        }
        /*
        scrp1.push_back(scores.first);
        scrp2.push_back(scores.second);
        xAxis.push_back(i);
        */
        delete starNN1;
        delete starNN2;
        delete ap2;
        //delete sga2;;
    }

    plotGameScores(scrp1, scrp2, xAxis, "test title");

    /*auto axes = CvPlot::makePlotAxes();
    axes.create<CvPlot::Series>(xAxis, scrp1, "-g");
    axes.create<CvPlot::Series>(xAxis, scrp2, "-b");
    CvPlot::show("mywindow", axes);

    cv::waitKey();
    */

    cout << "hello\n";
    return 0;
};

//**************** add UI


