#include "GameLauncher.h"


Position GameLauncher::makemove(IPlayer* player, int pl_id)
{
    Position mv(-1,-1);
    if (!game->hidden && !game->testmode)
        cout << "\n\nPLAYER "<<pl_id<<" move:\n";
    
    while (!(game->isvalidmove(mv)))
    {
        mv = player->makeMove(game->providedesk());
    }

    game->move(mv, pl_id);

    return mv;
}

GameLauncher::GameLauncher(Config newconfig)
{
	this->game = new Game(newconfig);
}

pair<int, int> GameLauncher::start(IPlayer* player1, IPlayer* player2)
{
    Position mv;
    int firstPlayerTurn = 1;

    player1->initValues(game->providedesk());
    player2->initValues(game->providedesk());
    while (!game->isgameover())
    {
        if (!(this->game->testmode) && !(this->game->hidden))
        {
            system("cls");
            game->displaydesk();
        }

        if (firstPlayerTurn)
        {
            mv=this->makemove(player1, 1);
            player1->updatePlayer(mv, game->providedesk());
            player2->updatePlayer(mv, game->providedesk());
        }
        else
        {
            mv=this->makemove(player2, 2);
            player1->updatePlayer(mv, game->providedesk());
            player2->updatePlayer(mv, game->providedesk());
        }

        if (!game->hidden && !game->testmode)
            cout << "MOVE: " << mv.row << " " << mv.col << endl;
        firstPlayerTurn ^= 1;
        auto scores = game->getScores();
        writeVectorToCSV({scores.first, scores.second }, "current_game_sccores.csv");

    }
    //system("cls");
    game->displaydesk();
    return game->getScores();
}

