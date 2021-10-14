#include "Player.h"

// Constructors
// Default constructor
Player::Player():
    playerTerritories(),
    playerCards(new Hand(*this)),
    playerOrdersList(new OrdersList()),
    playerArmies(0),
    playerID(0),
    territoriesToAttack(),
    territoriesToDefend()
{

}

//parametrized constructor
Player::Player(vector<Territory*>& playerTerritories, int playerArmies, int playerID, vector<Territory*>& territoriesToAttack, vector<Territory*>& territoriesToDefend):
    playerTerritories(playerTerritories),
    playerCards(new Hand(*this)),
    playerOrdersList(new OrdersList()),
    playerArmies(playerArmies),
    playerID(playerID),
    territoriesToAttack(territoriesToAttack),
    territoriesToDefend(territoriesToDefend)
{

}

// Copy constructor
Player::Player(const Player& other):
    playerTerritories(other.playerTerritories),
    playerArmies(other.playerArmies),
    playerID(other.playerID),
    territoriesToAttack(other.territoriesToAttack),
    territoriesToDefend(other.territoriesToDefend)
{
    delete this->playerCards;
    delete this->playerOrdersList;

    this->playerCards = new Hand(*(other.playerCards));
    this->playerOrdersList = new OrdersList(*(other.playerOrdersList));
}

// Destructor
Player::~Player()
{
    delete playerCards;
    delete playerOrdersList;
}

// Accessors
vector<Territory*>& Player::getPlayerTerritories()
{
    return playerTerritories;
}

Hand* Player::getPlayerCards()
{
    return playerCards;
}

OrdersList* Player::getPlayerOrders()
{
    return playerOrdersList;
}

int Player::getPlayerArmies() const
{
    return playerArmies;
}

// Mutators
void Player::setPlayerTerritories(vector<Territory*> playerTerritories)
{
    for (Territory* t : playerTerritories)
    {
        this->playerTerritories.push_back(t);
    }
}

void Player::setPlayerCards(Hand* playerCards)
{
    this->playerCards = playerCards;
}

void Player::setPlayerOrders(OrdersList* playerOrderList)
{
    this->playerOrdersList = playerOrderList;
}

void Player::setPlayerArmies(int playerArmies)
{
    this->playerArmies = playerArmies;
}

void Player::setTerritoriesToAttack(vector<Territory*> territoriesToAttack)
{
    for (Territory* t : territoriesToAttack)
    {
        this->territoriesToAttack.push_back(t);
    }
}

void Player::setTerritoriesToDefend(vector<Territory*> territoriesToDefend)
{
    for (Territory* t : territoriesToDefend)
    {
        this->territoriesToDefend.push_back(t);
    }
}

// Operator overloading
// Assignment operator overloading
void Player::operator = (const Player& other)
{
    delete playerCards;
    delete playerOrdersList;

    playerTerritories = other.playerTerritories;
    playerCards = new Hand(*(other.playerCards));
    playerOrdersList = new OrdersList(*(other.playerOrdersList));
    playerArmies = other.playerArmies;
    playerID = other.playerID;
    territoriesToAttack = other.territoriesToAttack;
    territoriesToDefend = other.territoriesToDefend;
}

//stream insertion operator overloading
ostream& operator << (ostream& out, const Player& source)
{
    out << "\nPlayer ID: " << source.playerID << endl;

    if (!source.playerTerritories.empty())
    {
        out << "\nPlayer's territories:\n";
        for (Territory *t : source.playerTerritories)
        {
            out << *t << endl;
        }
    }
    else
    {
        out << "\nTerritories not initialized!\n";
    }

    if (source.playerCards != nullptr)
    {
        out << "\nPlayer's Hand:\n";
        for (Card* c : source.playerCards->getCards())
        {
            out << *c << endl;
        }
    }
    else
    {
        out << "\nHand not initialized!\n";
    }

    if (source.playerOrdersList != nullptr)
    {
        out << "\nPlayer's Orders:\n";
        for (Order* o : source.playerOrdersList->getOrdersList())
        {
            out << *o;
        }
    }
    else
    {
        out << "\nOrders not initialized!\n";
    }

    return out;
}

// Required methods
void Player::issueOrder(Order* order)
{
    playerOrdersList->addOrder(order);
}

vector<Territory*>& Player::toDefend()
{
    return territoriesToDefend;
}

vector<Territory*>& Player::toAttack()
{
    return territoriesToAttack;
}
