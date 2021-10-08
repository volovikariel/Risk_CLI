#include "Player.h"

using namespace std;

//constructors
//default constructor
Player::Player() :

        playerTerritories(),
        playerCards(),
        playerOrdersList(),
        playerArmies(0),
        playerID(0),
        territoriesToAttack(),
        territoriesToDefend()
{

}

//parametrized constructor
Player::Player(vector<Territory*> playerTerritories_, Hand* playerCards_, OrdersList* playerOrders_, int playerArmies_, int playerID_, vector<Territory*> territoriesToAttack_, vector<Territory*> territoriesToDefend_) :
        playerTerritories(playerTerritories_),
        playerCards(playerCards_),
        playerOrdersList(playerOrders_),
        playerArmies(playerArmies_),
        playerID(playerID_),
        territoriesToAttack(territoriesToAttack_),
        territoriesToDefend(territoriesToDefend_)
{

}
//copy constructor
Player::Player(const Player& p) :
        playerTerritories(p.playerTerritories),
        playerArmies(p.playerArmies),
        playerID(p.playerID),
        territoriesToAttack(p.territoriesToAttack),
        territoriesToDefend(p.territoriesToDefend)
{
    delete this->playerCards;
    delete this->playerOrdersList;
    this->playerCards = new Hand(*(p.playerCards));
    this->playerOrdersList = new OrdersList(*(p.playerOrdersList));
}

//destructor
Player::~Player() {
    delete playerCards;
    delete playerOrdersList;
}

//accessors
vector<Territory*>& Player::getPlayerTerritories() {
    return playerTerritories;
}

Hand* Player::getPlayerCards() {
    return playerCards;
}

OrdersList* Player::getPlayerOrders() {
    return playerOrdersList;
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

void Player::setPlayerOrders(OrdersList* playerOrderList_)
{
    playerOrdersList = playerOrderList_;
}

void Player::addPlayerOrder(Order* playerOrder_) {
    if (playerOrdersList == nullptr) {
        playerOrdersList = new OrdersList();
    }
    playerOrdersList->addOrder(playerOrder_);
}

void Player::setPlayerArmies(int playerArmies_) {
    playerArmies = playerArmies_;
}

void Player::setTerritoriesToAttack(vector<Territory*> territoriesToAttack_) {
    for (Territory* t : territoriesToAttack_) {
        this->territoriesToAttack.push_back(t);
    }
}

void Player::setTerritoriesToDefend(vector<Territory*> territoriesToDefend_) {
    for (Territory* t : territoriesToDefend_) {
        this->territoriesToDefend.push_back(t);
    }
}

//operator overloading
//assignment operator overloading
void Player::operator=(const Player& p) {
    delete playerCards;
    delete playerOrdersList;

    playerTerritories = p.playerTerritories;
    playerCards = new Hand(*(p.playerCards));
    playerOrdersList = new OrdersList(*(p.playerOrdersList));
    playerArmies = p.playerArmies;
    playerID = p.playerID;
    territoriesToAttack = p.territoriesToAttack;
    territoriesToDefend = p.territoriesToDefend;
}

//stream insertion operator overloading
ostream& operator<<(ostream& out, const Player& p) {
    out << "\nPlayer ID: " << p.playerID << endl;

    if (!p.playerTerritories.empty()) {
        out << "\nPlayer's territories:\n";
        for (Territory *t : p.playerTerritories) {
            out << *t << endl;
        }
    }

    else {
        out << "\nTerritories not initialized!\n";
    }

    if (p.playerCards != nullptr) {
        out << "\nPlayer's Hand:\n";
        for (Card* c : p.playerCards->getCards()) {
            out << *c << endl;
        }
    }

    else {
        out << "\nHand not initialized!\n";
    }

    if (p.playerOrdersList != nullptr) {
        out << "\nPlayer's Orders:\n";
        for (Order* o : p.playerOrdersList->getOrdersList()) {
            out << *o;
        }
    }

    else {
        out << "\nOrders not initialized!\n";
    }

    return out;
}

//required methods
void Player::issueOrder(Order* order) {
    this->addPlayerOrder(order);
}

vector<Territory*>& Player::toDefend() {
    return territoriesToDefend;
}

vector<Territory*>& Player::toAttack() {
    return territoriesToAttack;
}