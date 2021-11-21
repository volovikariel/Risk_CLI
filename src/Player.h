#pragma once
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <stdlib.h>

using std::vector;

// Forward declarations
class GameEngine;
class Hand;
class PlayerStrategy;

class Player
{
public:

    // Constructor(s) and destructor
    Player();
    Player(const Player& other);
    ~Player();

    // Accessors
    // Returns the collection of territories of the player
    vector<Territory*>& getPlayerTerritories();
    // Returns the cards owned by the player
    Hand* getPlayerCards();
    // Returns the OrderList queued by the player
    OrdersList* getPlayerOrders();
    // Returns the armies of the player
    int getArmies() const;
    // Returns a list of territories that the player wants to defend based on the current strategy
    vector<Territory*>& toDefend(GameEngine& gameEngine);
    // Returns a list of territories that the player wants to attack based on the current strategy
    vector<Territory*>& toAttack(GameEngine& gameEngine);
    // Returns the player's name
    const std::string& getName() const;
    // Checks if player owns a specific territory
    bool hasTerritory(Territory* target);
    // Returns a list of players that the player can't attack
    vector<Player*>& getUnattackable();

    // Mutators
    void setPlayerTerritories(vector<Territory*> playerTerritories);
    void setPlayerCards(Hand* playerCards);
    void setPlayerOrders(OrdersList* playerOrderList);
    void setArmies(int armies);
    void setName(const std::string& name);
    void addPlayerTerritory(Territory* territory);
    bool removePlayerTerritory(Territory* territory);
    void setUnattackable(Player* player);
    void clearUnattackable();

    // Operator overloading
    // Assignment operator overloading
    void operator = (const Player& other);
    // Stream insertion operator overloading
    friend ostream& operator << (ostream& out, const Player& source);

    // Provides an order based on the current strategy
    Order* issueOrder(GameEngine& gameEngine);

    // Flag that grants a card on the next turn
    bool hasConqueredThisTurn;

    // Sets the issue order strategy
    void setPlayerStrategy(PlayerStrategy& strategy);
    // Gets the issue order strategy
    PlayerStrategy& getPlayerStrategy();

private:

    // Required attributes
    // Represents the collection of territories of the player
    vector<Territory*> playerTerritories;
    // Represents the collection of cards of the player
    Hand* playerCards;
    // Represents the OrderList queued by the player
    OrdersList* playerOrdersList;
    // Represents the collection of players that the player can't attack when a Negotiation Order is issued.
    vector<Player*> unattackable;

    // Helper attributes
    // Represent the number of armies of the player
    int armies;
    // Represents the player's name
    string name;

    // Strategy for issuing orders
    PlayerStrategy* strategy;
};
