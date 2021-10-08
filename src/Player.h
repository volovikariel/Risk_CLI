#pragma once
#include <iostream>
#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

using namespace std;
using std::vector;

class Player {
private:
    //required attributes
    vector<Territory*> playerTerritories; //represents the collection of territories of the player
    Hand* playerCards; //represents the collection of cards of the player
    OrdersList* playerOrdersList; //represents the OrderList queued by the player
    vector<Territory*> territoriesToAttack; //represents the collection of territories to attack
    vector<Territory*> territoriesToDefend; //represents the collection of territories to defend

    //helper attributes
    int playerArmies; //represent the number of armies of the player
    int playerID; //represents the player ID's

public:
    //constructor(s) and destructor
    Player();
    Player(vector<Territory*> playerTerritories_, Hand* playerCards_, OrdersList* playerOrderList_, int playerArmies_, int playerID_, vector<Territory*> territoriesToAttack_, vector<Territory*> territoriesToDefend_);
    Player(const Player& p);
    ~Player();

    //accessors
    vector<Territory*>& getPlayerTerritories(); //returns the collection of territories of the player
    Hand* getPlayerCards(); //returns the cards owned by the player
    OrdersList* getPlayerOrders(); //returns the OrderList queued by the player
    int getPlayerArmies(); //returns the armies of the player
    vector<Territory*>& toDefend(); //returns a list of territories owned by the player that need to be defended
    vector<Territory*>& toAttack(); //returns a list of territories that are attacked by the player

    //mutators
    void setPlayerTerritories(vector<Territory*> playerTerritories_);
    void setPlayerCards(Hand* playerCards_);
    void setPlayerOrders(OrdersList* playerOrderList_);
    void addPlayerOrder(Order* playerOrder_);
    void setPlayerArmies(int playerArmies_);
    void setTerritoriesToAttack(vector<Territory*> territoriesToAttack_);
    void setTerritoriesToDefend(vector<Territory*> territoriesToDefend_);

    //operator overloading
    //assignment operator overloading
    void operator=(const Player& p);
    //stream insertion operator overloading
    friend ostream& operator<<(ostream& out, const Player& p);

    //required methods
    void issueOrder(Order* order); //creates an Order object and adds it to playerOrdersList
};