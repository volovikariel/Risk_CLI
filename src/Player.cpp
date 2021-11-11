#include "Player.h"
#include "Cards.h"

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

const std::string& Player::getPlayerName() const
{
    return playerName;
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

void Player::setPlayerName(std::string& name)
{
    playerName = name;
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

// Return a list of territories that the player already owns to defend them.
vector<Territory*>& Player::toDefend()
{
    territoriesToDefend = this->getPlayerTerritories();
    return territoriesToDefend;
}

vector<Territory*>& Player::toAttack()
{
    vector<Territory*> toAttack = vector<Territory*>();

    // loop through each owned territory
    for (int i = 0; i < this->getPlayerTerritories().size(); i++){
        // get the adjacent territories to the current owned territory
        vector<Territory*> adjacentList = this->getPlayerTerritories().at(i)->neighbors;
        // loop through each adjacent territories
        for (int j = 0; j < adjacentList.size(); j++) {
            bool found = false;
            // for each territory that has been added to toAttack
            for (int k = 0; k < toAttack.size(); k++) {
                // make sure the current adjacentTerritory is not already in the list
                if (toAttack.at(k)->name.compare(adjacentList.at(j)->name) == 0) {
                    found = true;
                    break;
                }
            }
            // if the territoryname wasn't found
            if (!found) {
                toAttack.push_back(adjacentList.at(j));
            }
        }
    }
    territoriesToAttack = toAttack;
    return territoriesToAttack;
}
