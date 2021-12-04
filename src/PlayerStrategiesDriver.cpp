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
void testCheater(GameEngine& gameEngine);
void testNeutral(GameEngine& gameEngine);
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
    testCheater(gameEngine);
    testNeutral(gameEngine);
}

void testCheater(GameEngine& gameEngine)
{
    string mapFile = "../maps/canada.map";

    Player p1;
    p1.setName("NoStrategy");

    // Cheating Player
    Player p2;
    p2.setName("CheatingStrategy");
    CheaterPlayerStrategy cheatingStrategy(p2);
    p2.setPlayerStrategy(cheatingStrategy);

    vector<Player*> players = { &p1, &p2 };

    gameEngine.start(mapFile, players);

    gameEngine.executeTurn();
    gameEngine.executeTurn();
    gameEngine.executeTurn();

    cout << "testCheater";
}

void testNeutral(GameEngine& gameEngine)
{
    string mapFile = "../maps/canada.map";

    Player p1;
    p1.setName("NoStrategy");

    // Cheating Player
    Player p2;
    p2.setName("CheatingStrategy");
    CheaterPlayerStrategy cheatingStrategy(p2);
    p2.setPlayerStrategy(cheatingStrategy);

    // Neutral Player
    Player p3;
    p3.setName("NeutralStrategy");
    NeutralPlayerStrategy neutralStrategy(p3);
    p3.setPlayerStrategy(neutralStrategy);

    vector<Player*> players = { &p1, &p2, &p3 };

    gameEngine.start(mapFile, players);

    gameEngine.executeTurn();
    gameEngine.executeTurn();
    gameEngine.executeTurn();

    cout << "testCheater";
}

void test1(GameEngine& gameEngine)
{
    string mapFile = "../maps/canada.map";

    Player p1;
    p1.setName("NoStrategy");

    //Aggressive Player
    Player p2;
    p2.setName("AggressiveStrategy");
    AggressivePlayerStrategy aggressiveStrategy(p2);
    p2.setPlayerStrategy(aggressiveStrategy);

    //Cheating Player
    Player p3;
    p3.setName("CheatingStrategy");
    CheaterPlayerStrategy cheatingStrategy (p3);
    p3.setPlayerStrategy(cheatingStrategy);

    //Benevolent Player
    Player p4;
    p4.setName("BStrategy");
    BenevolentPlayerStrategy BenevolentStrategy(p4);
    p4.setPlayerStrategy(BenevolentStrategy);
   // p4.toDefend(gameEngine);
    //BenevolentStrategy.toDefend(gameEngine);

    //Neutral Player
    Player p5;
    p5.setName("NeutralStrategy");
    NeutralPlayerStrategy neutralPlayerStrategy(p5);
    p5.setPlayerStrategy(neutralPlayerStrategy);

    vector<Player*> players = { &p1, &p2, &p3, &p4 };

    gameEngine.start(mapFile, players);

    gameEngine.executeTurn();



    cout << "test";
}