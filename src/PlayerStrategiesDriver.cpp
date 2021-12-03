#include "CommandProcessing.h"
#include "GameEngine.h"
#include "LoggingObserver.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include "PlayerStrategies.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::vector;

// Declare test function signatures
void test1(GameEngine& gameEngine);

int main()
{
    // Log to file
    LogObserver logObserver;

    // Game engine
    GameEngine gameEngine;

    // Attach our observer
    gameEngine.attach(logObserver);

    // Launch tests
    test1(gameEngine);
}

void test1(GameEngine& gameEngine)
{
    string mapFile = "../maps/canada.map";

    Player p1;
    p1.setName("NoStrategy");

    Player p2;
    p2.setName("AggressiveStrategy");
    AggressivePlayerStrategy aggressiveStrategy(p2);
    p2.setPlayerStrategy(aggressiveStrategy);

    Player p3;
    p3.setName("CheatingStrategy");
    CheaterPlayerStrategy cheatingStrategy (p3);
    p3.setPlayerStrategy(cheatingStrategy);

    // Player p4;
    // p4.setName("BStrategy");
    // BenevolentPlayerStrategy BenevolentStrategy(p4);
    // p4.setPlayerStrategy(BenevolentStrategy);
   // p4.toDefend(gameEngine);
    //BenevolentStrategy.toDefend(gameEngine);

    vector<Player*> players = { &p1, &p2, &p3 };

    gameEngine.start(mapFile, players);

    gameEngine.executeTurn();




    cout << "test";
}