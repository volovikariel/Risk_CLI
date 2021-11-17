#include "Player.h"
#include "GameEngine.h"

// Constructors
// Default constructor
Player::Player():
    playerTerritories(),
    playerCards(new Hand(*this)),
    playerOrdersList(new OrdersList()),
    playerArmies(0),
    territoriesToAttack(),
    territoriesToDefend(),
    hasConqueredThisTurn(false)
{

}

// Parametrized constructor
Player::Player(vector<Territory*>& playerTerritories, int playerArmies, vector<Territory*>& territoriesToAttack, vector<Territory*>& territoriesToDefend):
    playerTerritories(playerTerritories),
    playerCards(new Hand(*this)),
    playerOrdersList(new OrdersList()),
    playerArmies(playerArmies),
    territoriesToAttack(territoriesToAttack),
    territoriesToDefend(territoriesToDefend),
    hasConqueredThisTurn(false)
{

}

// Copy constructor
Player::Player(const Player& other):
    playerTerritories(other.playerTerritories),
    playerArmies(other.playerArmies),
    territoriesToAttack(other.territoriesToAttack),
    territoriesToDefend(other.territoriesToDefend),
    hasConqueredThisTurn(other.hasConqueredThisTurn)
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

//Get the list of players that this player cannot attack
vector<Player*>& Player::getUnattackable()
{
    return unattackable;
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

void Player::setPlayerName(const std::string& name)
{
    playerName = name;
}

// Adds a new owned territory to player
void Player::addPlayerTerritory(Territory* territory)
{
    this->playerTerritories.push_back(territory);
}

bool Player::removePlayerTerritory(Territory* territory)
{
    std::vector<Territory*>::iterator it = std::find(playerTerritories.begin(), playerTerritories.end(), territory);
    if (it == playerTerritories.end())
    {
        return false;
    }
    else
    {
        playerTerritories.erase(it);
        return true;
    }
}

//Sets a player in the list of players that this player cannot attack
void Player::setUnattackable(Player* player)
{
    this->unattackable.push_back(player);
}

// Empties the unattackable vector of all players, to be called after every turn
void Player::clearUnattackable()
{
    this->unattackable.clear();
}

//Checks if a player has a target territory
bool Player::hasTerritory(Territory* target)
{
    return std::find(playerTerritories.begin(), playerTerritories.end(), target) != playerTerritories.end();
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
    territoriesToAttack = other.territoriesToAttack;
    territoriesToDefend = other.territoriesToDefend;
    hasConqueredThisTurn = other.hasConqueredThisTurn;
}

//stream insertion operator overloading
ostream& operator << (ostream& out, const Player& source)
{
    out << "\nPlayer Name: " << source.playerName << endl;

    out << "\nPlayer's Reinforcement Pool: " << source.playerArmies << endl;

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

void Player::newTurn()
{
    turnAttack = toAttack();
    turnDefend = toDefend();
    turnDeploy = toDefend();
    turnAdvances = min(3, static_cast<int>(playerTerritories.size()));
}

// Provides Player with an order based on available options
Order* Player::issueOrder(GameEngine& gameEngine)
{
    // If player still has available armies keep deploying
    if (playerArmies > 0)
    {
        int amount = 0;

        Territory* destination = turnDeploy.front();
        turnDeploy.erase(turnDeploy.begin());

        // Chose random amount of armies between 1 to 10
        amount = min(playerArmies, 1 + (rand() % 9));

        // Remove armies we will deploy from the player pool
        playerArmies -= amount;

        Deploy* deploy = new Deploy(amount, *this, *destination);
        return deploy;
    }
    // Choose a card and play it
    else if (!this->playerCards->getCards().empty())
    {
        Card* toPlay = this->getPlayerCards()->getCard(0);
        Card::Type cardType = toPlay->getType();
        toPlay->play();

        if (cardType == Card::Type::Airlift)
        {
            return playAirlift(gameEngine);
        }
        else if (cardType == Card::Type::Blockade)
        {
            return playBlockade(gameEngine);
        }
        else if (cardType == Card::Type::Bomb)
        {
            return playBomb(gameEngine);
        }
        else if (cardType == Card::Type::Diplomacy)
        {
            return playDiplomacy(gameEngine);
        }
        else if (cardType == Card::Type::Reinforcement)
        {
            playReinforcement();
        }
    }
    // If player has no cards do an advance order
    else if (turnAdvances > 0)
    {
        Territory* src;
        Territory* target;

        // Chose to either defend or attack
        int coinFlip = rand() % 2;

        // Attack
        if (coinFlip == 0)
        {
            target = turnAttack.front();
            for (auto option : target->neighbors)
            {
                if (option->player != this)
                {
                    turnAdvances--;
                    return new Advance(target->armies, *this, *target, *option);
                }
            }
            turnAttack.erase(turnAttack.begin());
        }
        // Defend
        else
        {
            target = turnDefend.front();
            for (auto option : target->neighbors)
            {
                if (option->player == this)
                {
                    turnAdvances--;
                    return new Advance(target->armies, *this, *target, *option);
                }
            }
            turnDefend.erase(turnDefend.begin());
        }
    }

    return nullptr;
}

Bomb* Player::playBomb(GameEngine& gameEngine)
{
    Territory* adajacentEnemyTerritory = nullptr;
    for (Territory* territory : toDefend())
    {
        for (Territory* neighbor : territory->neighbors)
        {
            if (neighbor->player != this && !gameEngine.isEliminated(neighbor->player))
            {
                adajacentEnemyTerritory = neighbor;
                break;
            }
        }
    }

    if (adajacentEnemyTerritory != nullptr)
    {
        return new Bomb(*this, *adajacentEnemyTerritory);
    }
    else
    {
        return nullptr;
    }
}

Airlift* Player::playAirlift(GameEngine& gameEngine)
{
    Territory* source = toDefend().back();
    Territory* destination = toDefend().front();
    return new Airlift(source->armies, *this, *source, *destination);
}

Blockade* Player::playBlockade(GameEngine& gameEngine)
{
    Territory* randomOwnTerritory = playerTerritories.at(rand() % playerTerritories.size());
    return new Blockade(*this, gameEngine.getNeutralPlayer(), *randomOwnTerritory);
}

Negotiate* Player::playDiplomacy(GameEngine& gameEngine)
{
    vector<Player*>& players = gameEngine.getPlayers();

    Player* randomPlayer;
    while (randomPlayer != this && !gameEngine.isEliminated(randomPlayer))
    {
        randomPlayer = players.at(rand() % players.size());
    }

    return new Negotiate(*this, *randomPlayer);
}

void Player::playReinforcement()
{
    playerArmies += 5;
}

// Returns a list of territories that the player already owns to defend them
vector<Territory*>& Player::toDefend()
{
    territoriesToDefend = this->getPlayerTerritories();
    return territoriesToDefend;
}

// Returns a list of territories that the player can attack
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
            //Only add to list if owned by enemy
            if (adjacentList.at(j)->player != this) {
                // for each territory that has been added to toAttack
                for (int k = 0; k < toAttack.size(); k++) {
                    // make sure the current adjacentTerritory is not already in the list
                    if (toAttack.at(k)->name.compare(adjacentList.at(j)->name) == 0) {
                        found = true;
                        break;
                    }
                }
                // if the territory name wasn't found
                if (!found) {
                    toAttack.push_back(adjacentList.at(j));
                }
            }
        }
    }
    territoriesToAttack = toAttack;
    return territoriesToAttack;
}
