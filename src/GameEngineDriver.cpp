#include "GameEngine.h"

#include <iostream>

int main()
{
    cout << "Welcome to the WarZone game.\nPlease follow the instructions." << endl;
    cout << "" <<endl;

    // Creating a game object to start the game
    GameEngine initialGame = GameEngine();

    // GameEngine object calling the function to start the game
    initialGame.gameEngineStart();

    return 0;
}
