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

const std::string& Player::getPlayerName() const
{
    return playerName;
}

int Player::getPlayerID() const {
    return playerID;
}

//Get the list of players that this player cannot attack
vector<Player *> Player::getUnattackable() {
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

void Player::setPlayerName(std::string& name)
{
    playerName = name;
}

// Adds a new owned territory to player
void Player::addPlayerTerritory(Territory* territory)
{
    this->playerTerritories.push_back(territory);
}

//Sets a player in the list of players that this player cannot attack
void Player::setUnattackable(vector<Player *> unattackble) {
    this->unattackable = unattackble;
}


//Checks if a player has a target territory
bool Player::hasTerritory(Territory* target) {
    if(std::find(playerTerritories.begin(), playerTerritories.end(), target) != playerTerritories.end()){
        return true;
    } else{
        return false;
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

// Old version of issue order based on return from old version of Card toPlay method (Maybe deprecated?)
void Player::issueOrder(Order* order)
{
    playerOrdersList->addOrder(order);
}

// Provides Player with an order based on available options
Order* Player::issueOrder() {
    srand(time(nullptr));

    // Prepare Territory options based on attacking or defending
    std::vector <Territory*> def = toDefend();
    std::vector<Territory *> atk = toAttack();

    // If player still has available armies keep deploying
    if (playerArmies > 0)
    {
        unsigned int amount = 0;

        Territory* destination = def[0];
        def.erase(def.begin());
        def.push_back(destination);

        // Chose random amount of armies between 1 to 10
        amount = 1 + (rand() % 9);

        if (amount > playerArmies){
            amount = playerArmies;
        }

        // Remove armies we will deploy from the player pool
        playerArmies -= amount;

        Deploy* deploy = new Deploy(amount, this, destination);
        return deploy;
    }
        // Chose card and play it.
    else {

        if (!this->playerCards->getCards().empty()) {

            Card* toPlay = this->getPlayerCards()->getCard(0);

            // Returns order based on Card
            Order* order = toPlay->play();

            // TODO Will need to set order info based on card type. (Using toAttack and toDefend)
            if (toPlay->getType() == Card::Type::Airlift) {
                cout << "Played Airlift" << endl;
            }
            if (toPlay->getType() == Card::Type::Blockade) {
                cout << "Played Blockade" << endl;
            }
            if (toPlay->getType() == Card::Type::Bomb) {
                cout << "Played Bomb" << endl;
            }
            if (toPlay->getType() == Card::Type::Diplomacy) {
                cout << "Played Diplomacy" << endl;
            }
            if (toPlay->getType() == Card::Type::Reinforcement) {
                cout << "Played Reinforcement" << endl;
            }
            return order;
        }
        //If player has no cards do an advance order
        else {
            Territory* src;
            Territory* target;

            // Chose to either defend or attack
            int r = rand()%2;

            if(r==0) {
                target = atk[0];
                for(auto option : target->neighbors) {
                    if (option->player == this && option->armies > 0)
                        src = option;
                    break;
                }
            }
            else {
                target = def[0];
                for(auto option : target->neighbors) {
                    if (option->player == this && option->armies > 0)
                        src = option;
                    break;
                }
            }

            // TODO Add proper Advance method "return new AdvanceOrder(this, src, target, src.armies);"
            cout << "Played Advance" << endl;
            return new Advance();
        }
    }
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
