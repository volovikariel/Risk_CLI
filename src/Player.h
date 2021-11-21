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
    vector<Territory*>& getTerritories();
    // Returns the cards owned by the player
    Hand* getCards();
    // Returns the orders queued by the player
    OrdersList* getOrders();
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
    void setTerritories(vector<Territory*>& territories);
    void addTerritory(Territory* territory);
    bool removeTerritory(Territory* territory);
    void setCards(Hand* cards);
    void setOrders(OrdersList* orders);
    void setArmies(int armies);
    void setName(const std::string& name);
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
    vector<Territory*> territories;
    // Represents the collection of cards of the player
    Hand* cards;
    // Represents the orders queued by the player
    OrdersList* orders;
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
