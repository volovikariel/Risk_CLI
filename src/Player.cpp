#include "Player.h"
#include "PlayerStrategies.h"
#include "GameEngine.h"

// Constructors
// Default constructor
Player::Player():
    territories(),
    cards(new Hand(*this)),
    orders(new OrdersList()),
    armies(0),
    hasConqueredThisTurn(false),
    strategy(nullptr)
{

}

// Copy constructor
Player::Player(const Player& other):
    territories(other.territories),
    armies(other.armies),
    hasConqueredThisTurn(other.hasConqueredThisTurn),
    strategy(other.strategy)
{
    delete this->cards;
    delete this->orders;

    this->cards = new Hand(*(other.cards));
    this->orders = new OrdersList(*(other.orders));
}

// Destructor
Player::~Player()
{
    delete cards;
    delete orders;
}

// Accessors
vector<Territory*>& Player::getTerritories()
{
    return territories;
}

Hand* Player::getCards()
{
    return cards;
}

OrdersList* Player::getOrders()
{
    return orders;
}

int Player::getArmies() const
{
    return armies;
}

const std::string& Player::getName() const
{
    return name;
}

//Get the list of players that this player cannot attack
vector<Player*>& Player::getUnattackable()
{
    return unattackable;
}



// Mutators
void Player::setTerritories(vector<Territory*>& territories)
{
    this->territories = territories;
}

void Player::setCards(Hand* cards)
{
    this->cards = cards;
}

void Player::setOrders(OrdersList* orders)
{
    this->orders = orders;
}

void Player::setArmies(int armies)
{
    this->armies = armies;
}

void Player::setName(const std::string& name)
{
    this->name = name;
}

// Adds a new owned territory to player
void Player::addTerritory(Territory* territory)
{
    territories.push_back(territory);
}

bool Player::removeTerritory(Territory* territory)
{
    std::vector<Territory*>::iterator it = std::find(territories.begin(), territories.end(), territory);
    if (it == territories.end())
    {
        return false;
    }
    else
    {
        territories.erase(it);
        return true;
    }
}

//Sets a player in the list of players that this player cannot attack
void Player::setUnattackable(Player* player)
{
    unattackable.push_back(player);
}

// Empties the unattackable vector of all players, to be called after every turn
void Player::clearUnattackable()
{
    unattackable.clear();
}

//Checks if a player has a target territory
bool Player::hasTerritory(Territory* target)
{
    return std::find(territories.begin(), territories.end(), target) != territories.end();
}

// Sets the issue order strategy
void Player::setPlayerStrategy(PlayerStrategy& strategy)
{
    this->strategy = &strategy;
}

// Gets the issue order strategy
PlayerStrategy& Player::getPlayerStrategy()
{
    return *strategy;
}

// Operator overloading
// Assignment operator overloading
void Player::operator = (const Player& other)
{
    delete cards;
    delete orders;

    territories = other.territories;
    cards = new Hand(*(other.cards));
    orders = new OrdersList(*(other.orders));
    armies = other.armies;
    hasConqueredThisTurn = other.hasConqueredThisTurn;
    strategy = other.strategy;
}

//stream insertion operator overloading
ostream& operator << (ostream& out, const Player& source)
{
    out << "\nPlayer Name: " << source.name << endl;

    out << "\nPlayer's Reinforcement Pool: " << source.armies << endl;

    if (!source.territories.empty())
    {
        out << "\nPlayer's territories:\n";
        for (Territory *t : source.territories)
        {
            out << *t << endl;
        }
    }
    else
    {
        out << "\nTerritories not initialized!\n";
    }

    if (source.cards != nullptr)
    {
        out << "\nPlayer's Hand:\n";
        for (Card* c : source.cards->getCards())
        {
            out << *c << endl;
        }
    }
    else
    {
        out << "\nHand not initialized!\n";
    }

    if (source.orders != nullptr)
    {
        out << "\nPlayer's Orders:\n";
        for (Order* o : source.orders->getOrdersList())
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

// Provides an order based on the current strategy
Order* Player::issueOrder(GameEngine& gameEngine)
{
    if (strategy != nullptr)
    {
        return strategy->issueOrder(gameEngine);
    }
    else
    {
        return nullptr;
    }
}

// Returns a list of territories that the player wants to defend them based on the current strategy
vector<Territory*>& Player::toDefend(GameEngine& gameEngine)
{
    return strategy->toDefend(gameEngine);
}

// Returns a list of territories that the player wants to attack based on the current strategy
vector<Territory*>& Player::toAttack(GameEngine& gameEngine)
{
    return strategy->toAttack(gameEngine);
}
