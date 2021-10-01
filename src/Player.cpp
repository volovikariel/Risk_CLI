#include "Player.h"

using namespace std;

//constructors
//default constructor
Player::Player() : playerTerritories(), playerCards(), playerOrderList(), playerArmies(0), playerID(0) {
}

//parametrized constructor
Player::Player(vector<Territory*> playerTerritories_, Hand* playerCards_, OrdersList* playerOrders_, int playerArmies_, int playerID_) : playerTerritories(playerTerritories_), playerCards(playerCards_), playerOrderList(playerOrders_), playerArmies(playerArmies_), playerID(playerID_) {
}
//copy constructor
Player::Player(const Player& p) : playerTerritories(p.playerTerritories), playerCards(new Hand(*(p.playerCards))), playerOrderList(new OrdersList(*(p.playerOrderList))), playerArmies(p.playerArmies), playerID(p.playerID) {
}

//destructor
Player::~Player() {
    for (Territory* t : playerTerritories) {
        delete t;
        t = nullptr;
    }

    delete playerCards;
    playerCards = nullptr;
    delete playerOrderList;
    playerOrderList = nullptr;

    playerArmies = 0;
    playerID = 0;
}

//accessors
vector<Territory*> Player::getPlayerTerritories() {
    return playerTerritories;
}

Hand* Player::getPlayerCards() {
    return playerCards;
}

OrdersList* Player::getPlayerOrders() {
    return playerOrderList;
}

int Player::getPlayerArmies() {
    return playerArmies;
}

//mutators
void Player::setPlayerTerritories(vector<Territory*> playerTerritories_) {
    for (Territory* t : playerTerritories_) {
        this->playerTerritories.push_back(t);
    }
}

void Player::setPlayerCards(Hand* playerCards_) {
    playerCards = playerCards_;
}

void Player::setPlayerOrders(Order* playerOrders_) {
    if (playerOrderList == nullptr) {
        playerOrderList = new OrdersList();
    }
    //playerOrderList->getOList().push_back(playerOrders_);
}

void Player::setPlayerArmies(int playerArmies_) {
    playerArmies = playerArmies_;
}

//operator overloading
//assignment operator overloading
void Player::operator=(const Player& p) {
    playerTerritories = p.playerTerritories;
    playerCards = new Hand(*(p.playerCards));
    playerOrderList = new OrdersList(*(p.playerOrderList));
    playerArmies = p.playerArmies;
    playerID = p.playerID;
}

//stream insertion operator overloading
ostream& operator<<(ostream& out, const Player& p) {
    out << "\nPlayer ID: " << p.playerID << endl;

    for (Territory* t : p.playerTerritories) {
        out << "Territories: " << *t << endl;
    }

    /*if (p.playerOrderList != nullptr) {
        for (Order* o : p.playerOrderList->getOList()) {
            out << "Orders: " << *o << endl;
        }
        out << "Orders should be printing here";
    }

    else {
        out << "\nOrders not initialized";
    }

    if (p.playerCards != nullptr) {
        for (Card* c : p.playerCards->get_hand()) {
            out << "\nCard: " << *c << endl;
        }
    }

    else {
        out << "\nHand not initialized";
    }*/

    return out;
}

istream& operator>>(istream& in, Player& p) {
    //in >> p.playerTerritories;
    //in >> p.playerCards;
    //in >> p.playerOrders;
    in >> p.playerID;
    return in;
}

//required methods
void Player::issueOrder(Order* order) {
    this->setPlayerOrders(order);
}

vector<Territory*> Player::toDefend() {
    Continent* c = new Continent(1, "North America", "Red", 100);
    Territory* t1 = new Territory(1, "Canada", c, 50, 100);
    Territory* t2 = new Territory(2, "United States", c, 100, 100);
    vector<Territory*> toDefend{ t1, t2 };
    return toDefend;
}

vector<Territory*> Player::toAttack() {
    Continent* c = new Continent(1, "Asia", "Red", 100);
    Territory* t1 = new Territory(1, "Russia", c, 1000, 100);
    Territory* t2 = new Territory(2, "China", c, 100, 50);
    vector<Territory*> toAttack{ t1, t2 };
    return toAttack;
}