#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Player.h"

#include <iostream>
#include <sstream>
#include <string>

int main()
{
    cout << "Welcome to the WarZone game.\nPlease follow the instructions." << endl;

    // Create a GameEngine object
    GameEngine gameEngine = GameEngine();

    // Handles user input and loadmap, validatemap, addplayer, gamestart steps
    gameEngine.startupPhase();

    // Display the player order (randomly determined)
    cout << "Random order of players:" << endl;
    for (const Player* player : gameEngine.getPlayers())
    {
        cout << "* " << player->getPlayerName() << endl;
    }

    // Display what resources the players have
    cout << endl << endl << endl;
    for (const Player* player : gameEngine.getPlayers())
    {
        cout << *player << endl << endl << endl;
    }
}
