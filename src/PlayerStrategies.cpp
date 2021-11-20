#include "PlayerStrategies.h"
#include "Orders.h"
#include "Player.h"

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
    out << "PlayerStrategy: " << source.player;
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
    out << "HumanPlayerStrategy: " << source.player;
    return out;
}

Order* HumanPlayerStrategy::issueOrder(GameEngine& gameEngine)
{
    return nullptr;
}

vector<Territory*>& HumanPlayerStrategy::toAttack(GameEngine& gameEngine)
{
    vector<Territory*> tmp;
    return tmp;
}

vector<Territory*>& HumanPlayerStrategy::toDefend(GameEngine& gameEngine)
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
    out << "HumanPlayerStrategy: " << source.player;
    return out;
}

Order* BenevolentPlayerStrategy::issueOrder(GameEngine& gameEngine)
{
    return nullptr;
}

vector<Territory*>& BenevolentPlayerStrategy::toAttack(GameEngine& gameEngine)
{
    vector<Territory*> tmp;
    return tmp;
}

vector<Territory*>& BenevolentPlayerStrategy::toDefend(GameEngine& gameEngine)
{
    vector<Territory*> tmp;
    return tmp;
}
