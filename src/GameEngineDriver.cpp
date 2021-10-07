#include "GameEngine.h"
#include <iostream>
using namespace std;

int main()
{
    cout<< "Welcome to the WarZone game.\nPlease follow the instructions."<<endl;
    cout<<""<<endl;
    GameEngine FirstGame =GameEngine();//creating a game object to start the game
    FirstGame.GameEngineStart();//GameEngine object calling the function to start the game
    return 0;
}
