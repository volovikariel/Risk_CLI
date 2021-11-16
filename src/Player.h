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
class Hand;

class Player
{
public:

    // Constructor(s) and destructor
    Player();
    Player(vector<Territory*>& playerTerritories, int playerArmies, int playerID, vector<Territory*>& territoriesToAttack, vector<Territory*>& territoriesToDefend);
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
    int getPlayerArmies() const;
    // Returns the player ID
    int getPlayerID() const;
    // Returns a list of territories owned by the player that need to be defended
    vector<Territory*>& toDefend();
    // Returns a list of territories that are attacked by the player
    vector<Territory*>& toAttack();
    // Returns the player's name
    const std::string& getPlayerName() const;
    // Checks if player owns a specific territory
    bool hasTerritory(Territory* target);
    // Returns a list of players that the player can't attack
    vector<Player *> getUnattackable();

    // Mutators
    void setPlayerTerritories(vector<Territory*> playerTerritories);
    void setPlayerCards(Hand* playerCards);
    void setPlayerOrders(OrdersList* playerOrderList);
    void setPlayerArmies(int playerArmies);
    void setTerritoriesToAttack(vector<Territory*> territoriesToAttack);
    void setTerritoriesToDefend(vector<Territory*> territoriesToDefend);
    void setPlayerName(const std::string& name);
    void addPlayerTerritory(Territory* territory);
    void setUnattackable(Player* player);
    void clearUnattackable();

    // Operator overloading
    // Assignment operator overloading
    void operator = (const Player& other);
    // Stream insertion operator overloading
    friend ostream& operator << (ostream& out, const Player& source);

    // Required methods
    // Creates an Order object and adds it to playerOrdersList
    void issueOrder(Order*);
    Order* issueOrder();

    bool hasConqueredThisTurn;

private:

    // Required attributes
    // Represents the collection of territories of the player
    vector<Territory*> playerTerritories;
    // Represents the collection of cards of the player
    Hand* playerCards;
    // Represents the OrderList queued by the player
    OrdersList* playerOrdersList;
    // Represents the collection of territories to attack
    vector<Territory*> territoriesToAttack;
    // Represents the collection of territories to defend
    vector<Territory*> territoriesToDefend;
    // Represents the collection of players that the player can't attack when a Negotiation Order is issued.
    vector<Player*> unattackable;

    // Helper attributes
    // Represent the number of armies of the player
    int playerArmies;
    // Represents the player ID's
    int playerID;
    // Represents the player's name
    string playerName;
};
