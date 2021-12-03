#include "PlayerStrategies.h"
#include "Orders.h"
#include "Player.h"

#include <algorithm>

// Utils

// Returns a list of territories that the player can attack
vector<Territory*> canAttack(Player& player)
{
    vector<Territory*> canAttack = vector<Territory*>();

    for (Territory* ownedTerritory : player.getTerritories())
    {
        for (Territory* neighbor : ownedTerritory->neighbors)
        {
            bool found = false;

            // Only add to list if not owned by self
            if (neighbor->player != &player)
            {
                // Make sure the current neighbor is not already in the list
                bool found = find(canAttack.begin(), canAttack.end(), neighbor) != canAttack.end();
                if (!found)
                {
                    canAttack.push_back(neighbor);
                }
            }
        }
    }

    return canAttack;
}

// If one of the last three orders was a Deploy order, you can keep deploying
bool canDeploy(Player& player)
{
    vector<Order*>& orders = player.getOrders()->getOrdersList();
    if((int)orders.size() < 3) return true;
    vector<Order*> lastThreeOrders(orders.end() - 3, orders.end());
    for(Order* order : lastThreeOrders)
    {
        if(order->getType() == Order::Type::Deploy)
        {
            return true;
        }
    }
    return false;
}

// PlayerStrategy

PlayerStrategy::PlayerStrategy()
{

}

PlayerStrategy::PlayerStrategy(Player& player):
    player(&player)
{

}

PlayerStrategy::PlayerStrategy(PlayerStrategy& other):
    player(other.player)
{

}

PlayerStrategy& PlayerStrategy::operator = (const PlayerStrategy& other)
{
    player = other.player;
    return *this;
}

std::ostream& operator << (std::ostream& out, const PlayerStrategy& source)
{
    out << "PlayerStrategy";
    return out;
}


// HumanPlayerStrategy

HumanPlayerStrategy::HumanPlayerStrategy():
    PlayerStrategy()
{

}

HumanPlayerStrategy::HumanPlayerStrategy(Player& player):
    PlayerStrategy(player)
{

}

HumanPlayerStrategy::HumanPlayerStrategy(HumanPlayerStrategy& other):
    PlayerStrategy(other)
{

}

HumanPlayerStrategy& HumanPlayerStrategy::operator = (const HumanPlayerStrategy& other)
{
    PlayerStrategy::operator=(other);
    return *this;
}

std::ostream& operator << (std::ostream& out, const HumanPlayerStrategy& source)
{
    out << "Human";
    return out;
}

Order* HumanPlayerStrategy::issueOrder(GameEngine& gameEngine)
{
    int orderNum;
    std::cout<<"Please input the corresponding number to the order you would like to issue from the list below.\n";
    std::cout<<"1) Deploy\n2) Advance\n3) Bomb\n4) Blockade\n5) Airlift\n6) Negotiate\n";
    std::cin>>orderNum;
    switch (orderNum)
    {
    case 1:
        std::cout<<"you chose Deploy\n";
        Deploy* deployTemp;
        do
        {
            int num_armies_available = this->player->getArmies();
            if(num_armies_available > 0 && canDeploy(*this->player)) 
            {
                vector<Territory*> ownedTerritories = this->player->getTerritories();
                std::cout<<"you have "<<num_armies_available<<" armies available to deploy. Please input the ID of the territory where you would like to deploy or input '0' to see a list of your owned territories:\n";
                int deployTerritoryId, deployNumArmies;
                std::cin>>deployTerritoryId;
                if (deployTerritoryId==0)
                {
                    for (Territory* territories: ownedTerritories)
                    {
                        std::cout<<territories;
                    }
                    std::cout<<"Please input the ID of the territory where you would like to deploy: \n";
                    std::cin>>deployTerritoryId;
                }
                Territory* deployTerritory = gameEngine.getMap().getTerritoryByID(deployTerritoryId);
                std::cout<<"How many armies would you like to deploy here? ("<<num_armies_available<<" available)\n";
                std::cin>>deployNumArmies;
                deployTemp= new Deploy(deployNumArmies,*this->player,*deployTerritory);
            }
        } while (!deployTemp->validate());
        return deployTemp;
        break;
    case 2:
        std::cout<<"you chose Advance";
        Advance* advanceTemp;
        do
        {
            int sourceTerritoryId,targetTerritoryId, armies;
            std::cout<<"Please input the ID of the territory where you would like to advance armies from or input '0' to see a list of your owned territories:\n";
            
            std::cin>>sourceTerritoryId;
            if (sourceTerritoryId==0)
            {
                vector<Territory*> ownedTerritories = this->player->getTerritories();
                for (Territory* territories: ownedTerritories)
                {
                    std::cout<<territories;
                }
                std::cout<<"Please input the ID of the territory where you would like to advance armies from: \n";
                std::cin>>sourceTerritoryId;
            }
            Territory* sourceTerritory = gameEngine.getMap().getTerritoryByID(sourceTerritoryId);

            std::cout<<"Please input the ID of the territory where you would like to advance armies to or input '0' to see a list of neighboring territories:\n";
            std::cin>>targetTerritoryId;
            if (targetTerritoryId==0)
            {
                
                for (Territory* territories: sourceTerritory->neighbors)
                {
                    std::cout<<territories;
                }
                std::cout<<"Please input the ID of the territory where you would like to advance armies to: \n";
                std::cin>>targetTerritoryId;
            }
            Territory* targetTerritory = gameEngine.getMap().getTerritoryByID(targetTerritoryId);
            std::cout<<"How many armies would you like to advance? ("<<sourceTerritory->armies<<" available)\n";
            std::cin>>armies;
            advanceTemp=new Advance(armies,*this->player,*sourceTerritory,*targetTerritory);
        } while (!advanceTemp->validate());
        return advanceTemp;
        //Advance(int armies, Player& player, Territory& sourceTerritory, Territory& targetTerritory, bool cheat=false);  
        break;
    case 3:
        std::cout<<"you chose Bomb";
        
        break;
    case 4:
        std::cout<<"you chose Blockade";
        break;
    case 5:
        std::cout<<"you chose Airlift";
        break;
    case 6:
        std::cout<<"you chose Negotiate";
        break;
    default:
        std::cout<<"That Input doesn't correspond to an order. Try again";
        break;
    }

    return nullptr;
}

vector<Territory*> HumanPlayerStrategy::toAttack(GameEngine& gameEngine)
{
    vector<Territory*> tmp;
    return tmp;
}

vector<Territory*> HumanPlayerStrategy::toDefend(GameEngine& gameEngine)
{
    vector<Territory*> tmp;
    return tmp;
}


// BenevolentPlayerStrategy

BenevolentPlayerStrategy::BenevolentPlayerStrategy():
    PlayerStrategy()
{

}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player& player):
    PlayerStrategy(player)
{

}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(BenevolentPlayerStrategy& other):
    PlayerStrategy(other)
{

}

BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator = (const BenevolentPlayerStrategy& other)
{
    PlayerStrategy::operator=(other);
    return *this;
}

std::ostream& operator << (std::ostream& out, const BenevolentPlayerStrategy& source)
{
    out << "Benevolent";
    return out;
}

Order* BenevolentPlayerStrategy::issueOrder(GameEngine& gameEngine)
{
    return nullptr;
}

vector<Territory*> BenevolentPlayerStrategy::toAttack(GameEngine& gameEngine)
{
    vector<Territory*> tmp;
    return tmp;
}

vector<Territory*> BenevolentPlayerStrategy::toDefend(GameEngine& gameEngine)
{
    vector<Territory*> tmp;
    return tmp;
}

// AggressivePlayerStrategy

AggressivePlayerStrategy::AggressivePlayerStrategy():
    PlayerStrategy()
{

}

AggressivePlayerStrategy::AggressivePlayerStrategy(Player& player):
    PlayerStrategy(player)
{

}

AggressivePlayerStrategy::AggressivePlayerStrategy(AggressivePlayerStrategy& other):
    PlayerStrategy(other)
{

}

AggressivePlayerStrategy& AggressivePlayerStrategy::operator = (const AggressivePlayerStrategy& other)
{
    PlayerStrategy::operator=(other);
    return *this;
}

std::ostream& operator << (std::ostream& out, const AggressivePlayerStrategy& source)
{
    out << "Aggressive";
    return out;
}

Order* AggressivePlayerStrategy::issueOrder(GameEngine& gameEngine)
{

    // ----- Friendly territories ----- //
    vector<Territory*> ownedTerritories = this->player->getTerritories();

    // --- Strongest --- //
    // First we find the iterator of Territory with the most armies
    // We're guaranteed that this won't be owned.end() because if the player had no territories, then the player would have already lost
    vector<Territory*>::iterator it_friendly_max = max_element(ownedTerritories.begin(), ownedTerritories.end(), [&](Territory* a, Territory* b) {
        return a->armies - b->armies;
    });
    int index_friendly_max = distance(ownedTerritories.begin(), it_friendly_max);
    Territory* strongest_friendly_territory = ownedTerritories.at(index_friendly_max);

    // --- Weakest --- //
    vector<Territory*>::iterator it_friendly_min = max_element(ownedTerritories.begin(), ownedTerritories.end(), [&](Territory* a, Territory* b) {
        return b->armies - a->armies;
    });
    int index_friendly_min = distance(ownedTerritories.begin(), it_friendly_min);
    Territory* weakest_friendly_territory = ownedTerritories.at(index_friendly_min);

    // --- Weakest enemy territory --- //
    vector<Territory*> enemyTerritories = toAttack(gameEngine); 
    vector<Territory*>::iterator it_enemy_min = max_element(enemyTerritories.begin(), enemyTerritories.end(), [&](Territory* a, Territory* b) {
        if(a->armies == 0) return -1;
        return b->armies - a->armies;
    });
    int index_enemy_min = distance(enemyTerritories.begin(), it_enemy_min);
    Territory* weakest_enemy_territory = enemyTerritories.at(index_enemy_min);

    
    // First deploy
    int num_armies_available = this->player->getArmies();
    if(num_armies_available > 0 && canDeploy(*this->player)) {
        // As an aggressive player, we put all our armies on our strongest territory (on our territory which contains the most units)
        return new Deploy(num_armies_available, *this->player, *strongest_friendly_territory);
    }

    // Try to play a card
    // The aggressive player only uses the Airlift, Reinforcement and Bomb cards
    Hand* playerHand = this->player->getCards();
    for (Card* card : playerHand->getCards())
    {
        Card::Type cardType = card->getType();

        Order* result = nullptr;

        if (cardType == Card::Type::Airlift)
        {
            // Move our weakest territory's armies to our strongest territory's armies as long as they're not the same
            if(weakest_friendly_territory != strongest_friendly_territory)
            {
                result = new Airlift(weakest_friendly_territory->armies, *this->player, *weakest_friendly_territory, *strongest_friendly_territory);
            }
        }
        else if (cardType == Card::Type::Bomb)
        {
            // Don't bomb territories that have no armies on them
            if(weakest_enemy_territory->armies != 0)
            {
                result = new Bomb(*this->player, *weakest_enemy_territory);
                card->play();
            }
        }
        else if (cardType == Card::Type::Reinforcement)
        {
            this->player->setArmies(this->player->getArmies() + 5);
            return nullptr;
        }

        if (result != nullptr)
        {
            return result;
        }
    }

    vector<Order*> orders = this->player->getOrders()->getOrdersList();
    // Advance with your strongest army onto enemy neighbours (attacking them)
    // Only advance if you haven't already queued an advance with from this territory
    bool alreadyAdvancedFromStrongestTerritory = find_if(orders.begin(), orders.end(), [&](Order* order) {
        return order->getType() == Order::Type::Advance && dynamic_cast<Advance*>(order)->getSourceTerritory() == strongest_friendly_territory;
    }) != orders.end();
    if(!alreadyAdvancedFromStrongestTerritory) {
        for(Territory* neighbouring_territory : strongest_friendly_territory->neighbors)
        {
            Player* neighbouring_territory_player = neighbouring_territory->player;
            if(neighbouring_territory_player != this->player && strongest_friendly_territory->armies > 0) {
                return new Advance(strongest_friendly_territory->armies, *this->player, *strongest_friendly_territory, *neighbouring_territory);
            }
        }
    }
    
    // If there are no enemy territories near your strongest territory, randomly attack with another of your territories
    // As long as this territory hasn't already issued an advance order this turn
    for(Territory* neighbour : enemyTerritories[0]->neighbors)
    {
        bool neighbour_is_owned = neighbour->player == this->player;
        bool alreadyAdvancedFromNeighbour = find_if(orders.begin(), orders.end(), [&](Order* order) {
            return order->getType() == Order::Type::Advance && dynamic_cast<Advance*>(order)->getSourceTerritory() == neighbour;
        }) != orders.end();
        if(neighbour_is_owned && neighbour->armies > 0 && !alreadyAdvancedFromNeighbour) {
            return new Advance(neighbour->armies, *this->player, *neighbour, *enemyTerritories[0]);
        }
    }
    
    // We shouldn't ever get here. If we didn't manage to find any territories that we can attack, then we have already won
    return nullptr;
}

vector<Territory*> AggressivePlayerStrategy::toAttack(GameEngine& gameEngine)
{
    vector<Territory*> tmp = canAttack(*this->player);
    return tmp;
}

vector<Territory*> AggressivePlayerStrategy::toDefend(GameEngine& gameEngine)
{
    // Don't defend, ever - just return an empty vector of Territories
    vector<Territory*> tmp;
    return tmp;
}


// CheaterPlayerStrategy

CheaterPlayerStrategy::CheaterPlayerStrategy():
        PlayerStrategy()
{

}

CheaterPlayerStrategy::CheaterPlayerStrategy(Player& player):
        PlayerStrategy(player)
{

}

CheaterPlayerStrategy::CheaterPlayerStrategy(CheaterPlayerStrategy& other):
        PlayerStrategy(other)
{

}

CheaterPlayerStrategy& CheaterPlayerStrategy::operator = (const CheaterPlayerStrategy& other)
{
    PlayerStrategy::operator=(other);
    return *this;
}

std::ostream& operator << (std::ostream& out, const CheaterPlayerStrategy& source)
{
    out << "Cheater";
    return out;
}

Order* CheaterPlayerStrategy::issueOrder(GameEngine& gameEngine)
{
    // Prepare list of adjacent territories the player will attack and conquer this turn
    vector<Territory*> territoriesToAttack = toAttack(gameEngine);

    // Since cheater player ignores armies when conquering we will just stack their armies on it first owned territory
    int num_armies_available = this->player->getArmies();
    if(num_armies_available > 0) {
        return new Deploy(num_armies_available, *this->player, *this->player->getTerritories().at(0));
    }

    // Get current amount of orders, to determine which territory we will attack assuming first order will always be deploy
    int pos = this->player->getOrders()->getOrdersList().size();
    if (pos <= territoriesToAttack.size()) {
        return new Advance(99, *this->player, *this->player->getTerritories().at(0), *territoriesToAttack.at(pos - 1),true);
        // All advance orders for all adjacent enemy territories are done so we stop issuing orders
    } else {
        return nullptr;
    }

}

// Returning all adjacent enemy territories the player can attack
vector<Territory*> CheaterPlayerStrategy::toAttack(GameEngine& gameEngine)
{
    vector<Territory*> tmp = canAttack(*this->player);
    return tmp;
}

// Cheater doesn't focus on defending so we return an empty list
vector<Territory*> CheaterPlayerStrategy::toDefend(GameEngine& gameEngine)
{
    vector<Territory*> tmp;
    return tmp;
}









//========== NeutralPlayerStrategy Class =========

//Default Constructor
NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy() {

}

//Parameterized Constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(Player &player) : PlayerStrategy(player) {

}

//Copy Constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(NeutralPlayerStrategy &other) : PlayerStrategy(other){

}

//Assignment Operator
NeutralPlayerStrategy &NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy &other) {
    PlayerStrategy::operator=(other);
    return *this;
}

//Input stream operator
std::ostream &operator<<(ostream &out, const NeutralPlayerStrategy &source) {
    out << "Neutral";
    return out;
}

//issueOrder()
//Does nothing. Issue order can't create any orders
Order *NeutralPlayerStrategy::issueOrder(GameEngine &gameEngine) {
    std::cout << this->player->getName() << " is a neutral player. Cannot issue any orders." << std::endl;
    return nullptr;
}

//toAttack()
vector<Territory *> NeutralPlayerStrategy::toAttack(GameEngine &gameEngine) {
    return vector<Territory *>();
}

//toDefend()
vector<Territory *> NeutralPlayerStrategy::toDefend(GameEngine &gameEngine) {
    return vector<Territory *>();
}
