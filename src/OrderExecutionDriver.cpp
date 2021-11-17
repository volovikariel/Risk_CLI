#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"

#include <iostream>
#include <vector>

// Automatic loadmap, validatemap, addplayer, gamestart without user input
void setupGame(GameEngine& gameEngine);

int main()
{
    GameEngine gameEngine;
    setupGame(gameEngine);
    std::cout << endl;

    Player& player1 = *gameEngine.getPlayers().at(0);
    Player& player2 = *gameEngine.getPlayers().at(1);
    Territory& player1Territory = *player1.getPlayerTerritories().at(0);
    Territory& player2Territory = *player2.getPlayerTerritories().at(0);

    // Deploy on other player's territory
    Deploy badDeploy(1, player1, player2Territory);
    badDeploy.execute();
    std::cout << badDeploy << endl;

    // Advance with source territory not belonging to the player
    Advance badAdvance(1, player1, player2Territory, player2Territory);
    badAdvance.execute();
    std::cout << badAdvance << endl;

    // Advance with target territory not adjacent
    Territory* notAdjacentPlayer2Territory = nullptr;
    // Find not adjacent territory belonging to the other player
    for (Territory* territory : player2.getPlayerTerritories())
    {
        if (!player1Territory.isNeighbor(territory))
        {
            notAdjacentPlayer2Territory = territory;
            break;
        }
    }
    if (notAdjacentPlayer2Territory != nullptr)
    {
        Advance badAdvance2(1, player1, player1Territory, *notAdjacentPlayer2Territory);
        badAdvance2.execute();
        std::cout << badAdvance2 << endl;
    }

    // Airlift from source territory that doesn't belong to player
    Airlift badAirlift(1, player1, player2Territory, player2Territory);
    badAirlift.execute();
    std::cout << badAirlift << endl;

    // Airlift to target territory that doesn't belong to player
    Airlift badAirlift2(1, player1, player1Territory, player2Territory);
    badAirlift2.execute();
    std::cout << badAirlift2 << endl;

    // Bombing oneself
    Bomb badBomb(player1, player1Territory);
    badBomb.execute();
    std::cout << badBomb << endl;

    // Bombing a territory not adjacent to one of ours
    std::vector<Territory*> backup = player2Territory.neighbors;
    player2Territory.neighbors.clear();
    Bomb badBomb2(player1, player2Territory);
    badBomb2.execute();
    std::cout << badBomb2 << endl;
    player2Territory.neighbors = backup;

    // Blockade a territory that doesn't belong to player
    Blockade badBlockade(player1, gameEngine.getNeutralPlayer(), player2Territory);
    badBlockade.execute();
    std::cout << badBlockade << endl;

    // Negotiate with oneself
    Negotiate badNegotiate(player1, player1);
    badNegotiate.execute();
    std::cout << badNegotiate << endl;

    std::cout << endl;

    // Conquer territory
    Territory* adjacentPlayer1Territory = nullptr;
    Territory* adjacentPlayer2Territory = nullptr;
    // Find adjacent territories belonging to different players
    for (Territory* t1 : player1.getPlayerTerritories())
    {
        for (Territory* t2 : player2.getPlayerTerritories())
        {
            if (t1->isNeighbor(t2))
            {
                adjacentPlayer1Territory = t1;
                adjacentPlayer2Territory = t2;
                break;
            }
        }
    }
    if (adjacentPlayer1Territory != nullptr && adjacentPlayer2Territory != nullptr)
    {
        // Test conquering

        adjacentPlayer1Territory->armies = 200;
        adjacentPlayer2Territory->armies = 2;

        Advance advanceAndConquer(200, player1, *adjacentPlayer1Territory, *adjacentPlayer2Territory);
        advanceAndConquer.execute();
        std::cout << advanceAndConquer << endl;

        std::cout << "Territory " << adjacentPlayer2Territory->name << " now belongs to " << adjacentPlayer2Territory->player->getPlayerName() << std::endl;
        std::cout << player1.getPlayerName() << " has conquered this turn flag: " << (player1.hasConqueredThisTurn ? "true" : "false") << std::endl;
        std::cout << player2.getPlayerName() << " has conquered this turn flag: " << (player2.hasConqueredThisTurn ? "true" : "false") << std::endl;

        std::cout << endl;

        // Test that player can't attack diplomatic ally

        // Reset territory ownership
        adjacentPlayer2Territory->player = &player2;
        player1.removePlayerTerritory(adjacentPlayer2Territory);
        player2.addPlayerTerritory(adjacentPlayer2Territory);

        Negotiate negotiateTest(player1, player2);
        negotiateTest.execute();
        std::cout << negotiateTest << endl;

        Advance badAdvance(200, player1, *adjacentPlayer1Territory, *adjacentPlayer2Territory);
        badAdvance.execute();
        std::cout << badAdvance << endl;
    }

    // Check that blockade gives territories to the neutral player
    Blockade blockadeTest(player1, gameEngine.getNeutralPlayer(), player1Territory);
    blockadeTest.execute();
    std::cout << blockadeTest << endl;
    std::cout << player1Territory.name << " now belongs to player " << player1Territory.player->getPlayerName();

    std::cout << endl;
}

void setupGame(GameEngine& gameEngine)
{
    // Startup parameters
    const char* mapFilePath = "../maps/canada.map";
    const int numPlayers = 2;


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
        return;
    }


    // Validate map
    result = gameEngine.getMap().validate();

    std::cout << "Validation error: " << result << " for map " << gameEngine.getMap().name << std::endl;

    if (result != Map::FormatError::None)
    {
        return;
    }


    // Add players
    static const char* playerNames[] = { "player1", "player2", "player3", "player4", "player5", "player6" };
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
    for (size_t i = 0; i < gameEngine.getMap().territories.size(); ++i)
    {
        Player* playerToAssign = gameEngine.getPlayers().at(i % numPlayers);

        Territory* territory = gameEngine.getMap().territories.at(i);

        territory->player = playerToAssign;
        playerToAssign->addPlayerTerritory(territory);
    }


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
}
