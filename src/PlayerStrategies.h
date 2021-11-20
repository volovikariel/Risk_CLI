#pragma once
#include "GameEngine.h"
#include "Map.h"

#include <vector>
using std::vector;

class Order;
class Player;

class PlayerStrategy
{
public:

    PlayerStrategy();
    PlayerStrategy(Player& player);
    PlayerStrategy(PlayerStrategy& other);

    PlayerStrategy& operator = (const PlayerStrategy& other);
    friend std::ostream& operator << (std::ostream& out, const PlayerStrategy& source);

    virtual Order* issueOrder(GameEngine& gameEngine) = 0;
    virtual vector<Territory*>& toAttack(GameEngine& gameEngine) = 0;
    virtual vector<Territory*>& toDefend(GameEngine& gameEngine) = 0;

    Player* player;
};

class HumanPlayerStrategy : public PlayerStrategy
{
public:

    HumanPlayerStrategy();
    HumanPlayerStrategy(Player& player);
    HumanPlayerStrategy(HumanPlayerStrategy& other);

    HumanPlayerStrategy& operator = (const HumanPlayerStrategy& other);
    friend std::ostream& operator << (std::ostream& out, const HumanPlayerStrategy& source);

    Order* issueOrder(GameEngine& gameEngine) override;
    vector<Territory*>& toAttack(GameEngine& gameEngine) override;
    vector<Territory*>& toDefend(GameEngine& gameEngine) override;
};

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:

    BenevolentPlayerStrategy();
    BenevolentPlayerStrategy(Player& player);
    BenevolentPlayerStrategy(BenevolentPlayerStrategy& other);

    BenevolentPlayerStrategy& operator = (const BenevolentPlayerStrategy& other);
    friend std::ostream& operator << (std::ostream& out, const BenevolentPlayerStrategy& source);

    Order* issueOrder(GameEngine& gameEngine) override;
    vector<Territory*>& toAttack(GameEngine& gameEngine) override;
    vector<Territory*>& toDefend(GameEngine& gameEngine) override;
};
