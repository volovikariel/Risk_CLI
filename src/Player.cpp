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

int Player::getPlayerID() const {
    return playerID;
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

void Player::addPlayerTerritory(Territory* territory)
{
    this->playerTerritories.push_back(territory);
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

void Player::issueOrder(Order* order)
{
    playerOrdersList->addOrder(order);
}

Order* Player::issueOrder() {
    srand(time(nullptr));

    std::vector <Territory*> def = toDefend();
    std::vector<Territory *> atk = toAttack();

    // If player still has available armies keep deploying
    if (playerArmies > 0)
    {

        Territory* destination = def[0];
        def.erase(def.begin());
        def.push_back(destination);

        unsigned int amount = 1 + (rand() % 9);

        if (amount > playerArmies){
            amount = playerArmies;
        }

        playerArmies -= amount;

        // Need to update
        Deploy* deploy = new Deploy();
        return deploy;
    }
    // Chose card and play it.
    else {

        if (!this->playerCards->getCards().empty()) {


            Card *toPlay = this->playerCards->getCard(0);
            this->getPlayerCards()->removeCard(0);
            // no Clue if this method will be the one to use for make the order
            //toPlay->play();

            // Might need different order info depending on type (Need to implement when order are complete)
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

            delete toPlay;
            // modify with order type when order logic is complete
            return new Order();
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

            //return new AdvanceOrder(this, src, target, src.armies);
            cout << "Played Advance" << endl;
            return new Order();
        }
    }
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
                // if the territoryname wasn't found
                if (!found) {
                    toAttack.push_back(adjacentList.at(j));
                }
            }
        }
    }
    territoriesToAttack = toAttack;
    return territoriesToAttack;
}
