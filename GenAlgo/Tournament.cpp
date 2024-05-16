#include "Tournament.h"

pair<int, int> Tournament::runSingleGame(ShortPatternSet pattern1, ShortPatternSet pattern2, Config& config)
{
        pair<int, int> scores;
        GameLauncher mynewgame(config);

        ShortGA* sp1, * sp2;

        sp1 = new ShortGA(1, pattern1);
        sp2 = new ShortGA(2, pattern2);

        scores = mynewgame.start(sp1, sp2);
        delete sp1;
        delete sp2;
        return scores;
}

vector<vector<int>> Tournament::tournamentPiramide(vector<ShortPatternSet> patternSets, bool hidden)
{
    Config config;
    //GameLauncher mygame(config);

    int maxLines = config.getheight() * config.getwidth() / config.getlinesize() / 2;

    bool useThreads = 0;
    int nPlayers = patternSets.size();

    int victoryTreashold = 5;

    vector<vector<int>> tTable(nPlayers, vector<int>(nPlayers, 0));
    vector<int> currentPlayers, nextPlayers;
    for (int i = 0; i < nPlayers; i++)
    {
        currentPlayers.push_back(i);
    }
   if(!hidden)
       cout << "n= " << nPlayers << endl;

    int cnt = 0;
    bool isNextRound = true;

    while (isNextRound)
    {
        isNextRound = false;

        if (currentPlayers.size() % 2 == 1)
            nextPlayers.push_back(currentPlayers.back());
        currentPlayers.pop_back();

        int currentNumPlayers = currentPlayers.size();

#pragma omp parallel for schedule(dynamic, 1) shared(cnt)
        for (int i = 0; i < currentNumPlayers / 2; i++)
        {
            pair<int, int> victoriesCnt = { 0, 0 };

            int player1Id = currentPlayers[i];
            int player2Id = currentPlayers[currentNumPlayers - i - 1];
            pair<int, int> scores;
            vector<pair<int, int>> scoreLog;

            //scores = runSingleGame(patternSets[player1Id], patternSets[player2Id], config);
            //scoreLog.push_back(scores);

            bool isConfidentVictory = false;

            for (int t = 0; t < 3 && !isConfidentVictory; t++)
            {
                scores = runSingleGame(patternSets[player1Id], patternSets[player2Id], config);
                scoreLog.push_back(scores);
                if (scores.first > scores.second)
                    victoriesCnt.first++;
                else
                    victoriesCnt.second++;

                if (t == 0 && abs(scores.first - scores.second) >= victoryTreashold)
                    isConfidentVictory = true;
            }


            if (victoriesCnt.first > victoriesCnt.second)
            {
                nextPlayers.push_back(player1Id);
            }
            else
            {
                nextPlayers.push_back(player2Id);
            }


            scores = { 0, 0 };
            for (auto sc : scoreLog)
            {
                scores.first += sc.first;
                scores.second += sc.second;
            }
            scores.first /= scoreLog.size();
            scores.second /= scoreLog.size();

            tTable[player1Id][player2Id] = scores.first;
            tTable[player2Id][player1Id] = scores.second;

            if (!hidden)
            {
#pragma omp critical
                {
                    cout << cnt++ << ":  " << i << " / " << currentNumPlayers / 2 << " ) ";
                    cout << "SCORES: " << scores.first << " " << scores.second << endl;
                }
            }

        }

        if (nextPlayers.size() > 1)
        {
            isNextRound = true;
            currentPlayers = nextPlayers;
            nextPlayers.clear();
        }
    }

    if (!hidden)
    {

        cin.clear();
        cout << cnt << " games\n";
        cout << "---\n";

        int highScore = 0;
        int highDiff = 0;
        for (int i = 0; i < nPlayers; i++)
        {
            for (int j = 0; j < nPlayers; j++)
            {
                highScore = max(highScore, tTable[i][j]);
                highDiff = max(highDiff, tTable[i][j] - tTable[j][i]);
                if (!hidden)
                    cout << tTable[i][j] << "\t";
            }
            if (!hidden)
                cout << endl;
        }

        cout << "---\n";
        cout << "highscore: " << highScore << "\tdif: " << highDiff << endl;

        std::string filename = "highscores.csv";

        std::fstream fs;
        fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

        fs << highScore << ", " << highDiff << "\n";
        fs.close();
    }

    return tTable;
}