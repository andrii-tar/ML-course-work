// GenAlgo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <chrono>

#include"GameEnv.h"
#include"Players.h"

using namespace std;

#include"Tournament.h"

#include"Evolution.h"


#include"Eigen/Dense"
using namespace Eigen;


int main()
{
    srand(time(NULL));

    Config config;
    config.hidden = 1;
    GameLauncher mygame(config);

    HumanPlayer* pl1, * pl2;
    RandomPlayer* rp1, * rp2;
    //  AlgoPlayer* ap1, * ap2;

    int numThreads = 4;
    omp_set_num_threads(numThreads);



    pl1 = new HumanPlayer(1);
    pl2 = new HumanPlayer(2);

    rp1 = new RandomPlayer(1);
    rp2 = new RandomPlayer(2);


    // ShortPatternSet sps(5);
     //ShortGA* sga = new ShortGA(1, sps);

    EvoConfig evoConf;
    Evolution evolution(evoConf);

    vector<int> scoresPlayer1, scoresPlayer2;
    vector<pair<int, int>>testScores;
    vector<int> xAxis;


    vector<ShortPatternSet> currentGen(evoConf.populationSize);
    vector<ShortPatternSet> nextGen;

    bool recreate = false;
    if (recreate) {

        for (int i = 0; i < evoConf.populationSize; i++)
            currentGen[i] = ShortPatternSet(config.getlinesize());

        ShortPatternSet::savePatternSets(currentGen);
    }

    currentGen = ShortPatternSet::loadPatternSets(evoConf.populationSize, config.getlinesize());
    bool runDemoGame = true;

    if (runDemoGame)
    {
        int t;
        config.hidden = false;
        AlgoPlayer* ap2 = new AlgoPlayer(2);
        AlgoPlayer* ap1 = new AlgoPlayer(1);
        GreedyAlgoPlayer* gap2 = new GreedyAlgoPlayer(2);
        ShortGA* sga = new ShortGA(1, currentGen[0]);
        ShortGA* sga2 = new ShortGA(2, currentGen[0]);
        GameLauncher gameLauncher(config);
        auto res = gameLauncher.start(sga, sga2);
        cout << "\tresult: " << res.first << " " << res.second << endl;
        cin >> t;

        sga = new ShortGA(1, currentGen[0]);
        ShortGA* sg2 = new ShortGA(2, currentGen[1]);
        gameLauncher = GameLauncher(config);
        res = gameLauncher.start(sga, sg2);
        cout << "\tresult: " << res.first << " " << res.second << endl;

        config.hidden = true;
        cin >> t;
    }



    int numIters = 1500;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int iter = 0; iter < numIters; iter++)
    {
        bool logSelectedPopulation = (iter % 10 == 0);
        bool saveCurrentPatterns = (iter % 50 == 0);
        vector<vector<int>> tTable = Tournament::tournamentPiramide(currentGen);
        nextGen = evolution.repopulate(tTable, currentGen, logSelectedPopulation);


        currentGen = nextGen;
        if (iter % 5 == 0)
        {
            AlgoPlayer* ap2 = new AlgoPlayer(2);
            ShortGA* sga = new ShortGA(1, currentGen[0]);
            GameLauncher gameLauncher(config);
            auto res = gameLauncher.start(sga, ap2);
            cout << "Iteration: " << iter << " / " << numIters;
            cout << "\tresult: " << res.first << " " << res.second;
            if (res.first > res.second)
                cout << " - ";
            cout << endl;

          //  testScores.push_back(res);
            scoresPlayer1.push_back(res.first);
            scoresPlayer2.push_back(res.second);
            xAxis.push_back(iter);

            writeVectorToCSV({ iter, res.first, res.second }, SHORT_GA_LEARNING_SCORES);
            delete ap2;
            delete sga;
        }

        if (saveCurrentPatterns)
        {
            ShortPatternSet::saveSinglePatternSet(nextGen[0], "iter_" + to_string(iter));
            ShortPatternSet::savePatternSets(nextGen);
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::minutes> (end - begin).count() << "min" << std::endl;


    ShortPatternSet::savePatternSets(currentGen);
    ShortPatternSet::saveSinglePatternSet(currentGen[0], "iter_" + to_string(numIters));

   // for (auto res : testScores) {
   //     cout << "result: " << res.first << " " << res.second << endl;
   // }
    plotGameScores(scoresPlayer1, scoresPlayer2, xAxis, "Genetic algo vs AlgoBot");

    cout << "hello\n";
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
