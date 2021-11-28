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

// TODO: Returns true if we can keep issuing orders (if there's no previous deploy order or if we did at most 2 non-deploy orders after the last deploy order)

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
        return b->armies - a->armies;
    });
    int index_enemy_min = distance(enemyTerritories.begin(), it_enemy_min);
    Territory* weakest_enemy_territory = enemyTerritories.at(index_enemy_min);

    
    // First deploy
    int num_armies_available = this->player->getArmies();
    if(num_armies_available > 0) {
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
            // Move our weakest territory's armies to our strongest territory's armies
            result = new Airlift(weakest_friendly_territory->armies, *this->player, *weakest_friendly_territory, *strongest_friendly_territory);
        }
        else if (cardType == Card::Type::Bomb)
        {
            result = new Bomb(*this->player, *weakest_enemy_territory);
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

    // Advance with your strongest army onto enemy neighbours (attacking them)
    for(Territory* neighbouring_territory : strongest_friendly_territory->neighbors)
    {
        Player* neighbouring_territory_player = neighbouring_territory->player;
        if(neighbouring_territory_player != this->player) {
            return new Advance(strongest_friendly_territory->armies, *this->player, *strongest_friendly_territory, *neighbouring_territory);
        }
    }
    // If there are no enemy territories near it, just randomly attack with another of your territories
    for(Territory* neighbour : enemyTerritories[0]->neighbors)
    {
        bool neighbour_is_owned = neighbour->player == this->player;
        if(neighbour_is_owned) {
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
    // Prioritize: Don't defend, ever - just return an empty vector of Territories
    vector<Territory*> tmp;
    return tmp;
}