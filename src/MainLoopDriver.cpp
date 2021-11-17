#include "GameEngine.h"
#include "Cards.h"
#include "Map.h"
#include "Player.h"

#include <iostream>

int main()
{
    std::cout << "======================================= Part 3  ======================================= " << std::endl;

    // Startup parameters
    const char* mapFilePath = "../maps/redvsblue.map";
    const int numPlayers = 2;


    GameEngine gameEngine;


    // Filling the deck with cards of all types
    mainDeck.addCard(*new Card(Card::Type::Airlift));
    mainDeck.addCard(*new Card(Card::Type::Blockade));
    mainDeck.addCard(*new Card(Card::Type::Bomb));
    mainDeck.addCard(*new Card(Card::Type::Diplomacy));
    mainDeck.addCard(*new Card(Card::Type::Reinforcement));
    mainDeck.addCard(*new Card(Card::Type::Airlift));
    mainDeck.addCard(*new Card(Card::Type::Blockade));
    mainDeck.addCard(*new Card(Card::Type::Bomb));
    mainDeck.addCard(*new Card(Card::Type::Diplomacy));
    mainDeck.addCard(*new Card(Card::Type::Reinforcement));
    mainDeck.addCard(*new Card(Card::Type::Airlift));
    mainDeck.addCard(*new Card(Card::Type::Blockade));
    mainDeck.addCard(*new Card(Card::Type::Bomb));
    mainDeck.addCard(*new Card(Card::Type::Diplomacy));
    mainDeck.addCard(*new Card(Card::Type::Reinforcement));

    // Load map
    int errorLine;
    MapLoader mapLoader;
    Map::FormatError result = mapLoader.load(mapFilePath, gameEngine.getMap(), errorLine);

    if (errorLine > 0)
    {
        std::cout << "Load error: " << result << " on line " << errorLine;
    }
    else
    {
        std::cout << "Load error: " << result;
    }
    std::cout << " for map " << gameEngine.getMap().name << std::endl;

    if (result != Map::FormatError::None)
    {
        return 1;
    }


    // Validate map
    result = gameEngine.getMap().validate();

    std::cout << "Validation error: " << result << " for map " << gameEngine.getMap().name << std::endl;

    if (result != Map::FormatError::None)
    {
        return 1;
    }


    // Add players
    static const char* playerNames[] = {"player1", "player2", "player3", "player4", "player5", "player6"};
    for (int i = 0; i < numPlayers; ++i)
    {
        std::string playerName = playerNames[i];
        vector<Territory*> territories;
        vector<Territory*> territoriesToDefend;
        vector<Territory*> territoriesToAttack;


        Player* newPlayer = new Player(territories, 0, territoriesToAttack, territoriesToDefend);
        newPlayer->setPlayerName(playerName);

        gameEngine.getPlayers().push_back(newPlayer);
    }


    // Distribute territories
    for (int i = 0; i < gameEngine.getMap().territories.size(); ++i)
    {
        Player* playerToAssign = gameEngine.getPlayers().at(i % numPlayers);

        Territory* territory = gameEngine.getMap().territories.at(i);

        territory->player = playerToAssign;
        playerToAssign->addPlayerTerritory(territory);
    }

    //Hardcode for full continent bonus test
    //gameEngine.getPlayers().front()->addPlayerTerritory(gameEngine.getMap().territories.at(1));
    //gameEngine.getPlayers().front()->addPlayerTerritory(gameEngine.getMap().territories.at(3));


    // Giving initial 50 reinforcements to players
    for (Player* player : gameEngine.getPlayers())
    {
        player->setPlayerArmies(50);
    }


    // Drawing 2 initial cards for each player
    for (Player* player : gameEngine.getPlayers())
    {
        player->setPlayerCards(new Hand(*player));

        Card* drawnCard1 = mainDeck.draw();
        Card* drawnCard2 = mainDeck.draw();
        player->getPlayerCards()->addCard(*drawnCard1);
        player->getPlayerCards()->addCard(*drawnCard2);
    }


    // Fast-forward states
    gameEngine.transition(GameEngine::Transition::LoadMap);
    gameEngine.transition(GameEngine::Transition::ValidateMap);
    gameEngine.transition(GameEngine::Transition::AddPlayer);
    gameEngine.transition(GameEngine::Transition::GameStart);


    std::cout << "Ready with " << numPlayers << " players" << std::endl;

    try {
        gameEngine.mainGameLoop();
    }
    catch (exception& ex) {
        cout << "Error starting game! " << ex.what();
    }


    std::cout << "======================================= Part 3 end =======================================\n" << std::endl;
}
