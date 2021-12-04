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
void testAggressive(GameEngine& gameEngine);
void testNeutral(GameEngine& gameEngine);

int main()
{
    // Log to file
    LogObserver logObserver;

    // Game engine
    GameEngine gameEngine;

    // Attach our observer
    gameEngine.attach(logObserver);

    // Launch tests
    logObserver.appendLine("TESTING CHEATER");
    testCheater(gameEngine);

    logObserver.appendLine("\n\n\n\nTESTING AGGRESSIVE");
    testAggressive(gameEngine);

    logObserver.appendLine("\n\n\n\nTESTING NEUTRAL");
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

    cout << "testCheater" << endl;
}

void testAggressive(GameEngine& gameEngine)
{
    string mapFile = "../maps/canada.map";

    Player p1;
    p1.setName("NoStrategy");

    // Aggressive Player 1
    Player p2;
    p2.setName("AggressiveStrategy_1");
    AggressivePlayerStrategy aggressiveStrategy_1(p2);
    p2.setPlayerStrategy(aggressiveStrategy_1);

    // Neutral Player
    Player p3;
    p3.setName("AggressiveStrategy_2");
    AggressivePlayerStrategy aggressiveStrategy_2(p3);
    p3.setPlayerStrategy(aggressiveStrategy_2);

    vector<Player*> players = { &p1, &p2, &p3 };

    gameEngine.start(mapFile, players);

    gameEngine.executeTurn();
    gameEngine.executeTurn();
    gameEngine.executeTurn();

    cout << "testAggressive" << endl;
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

    cout << "testNeutral" << endl;
}
