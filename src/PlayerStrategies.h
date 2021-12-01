#pragma once
#include "GameEngine.h"
#include "Map.h"

#include <vector>
using std::vector;

class Order;
class Player;


//========== PlayerStrategy (Abstract) Class ==========//

class PlayerStrategy
{
public:

    PlayerStrategy(); // Default Constructor
    PlayerStrategy(Player& player); // Parametrized Constructor
    PlayerStrategy(PlayerStrategy& other); // Parametrized Constructor

    PlayerStrategy& operator = (const PlayerStrategy& other); // Assignment operator overloading
    friend std::ostream& operator << (std::ostream& out, const PlayerStrategy& source); // Input stream operator

    virtual Order* issueOrder(GameEngine& gameEngine) = 0;  // Pure virtual method issueOrder()
    virtual vector<Territory*> toAttack(GameEngine& gameEngine) = 0; // Pure virtual method toAttack()
    virtual vector<Territory*> toDefend(GameEngine& gameEngine) = 0; // Pure virtual method toDefend()

    Player* player;
};


//========== Human PlayerStrategy Class ==========//

class HumanPlayerStrategy : public PlayerStrategy
{
public:

    HumanPlayerStrategy();  // Default Constructor
    HumanPlayerStrategy(Player& player);    // Parametrized Constructor
    HumanPlayerStrategy(HumanPlayerStrategy& other);    // Parametrized Constructor

    HumanPlayerStrategy& operator = (const HumanPlayerStrategy& other); // Assignment operator overloading
    friend std::ostream& operator << (std::ostream& out, const HumanPlayerStrategy& source); // Input stream operator

    Order* issueOrder(GameEngine& gameEngine) override; // Overriding virtual function issueOrder() from base class PlayerStrategy
    vector<Territory*> toAttack(GameEngine& gameEngine) override;   // Overriding virtual function toAttack() from base class PlayerStrategy
    vector<Territory*> toDefend(GameEngine& gameEngine) override;   // Overriding virtual function toDefend() from base class PlayerStrategy
};



//========== Benevolent PlayerStrategy Class ==========//

class BenevolentPlayerStrategy : public PlayerStrategy
{
public:

    BenevolentPlayerStrategy(); // Default Constructor
    BenevolentPlayerStrategy(Player& player);   // Parameterized Constructor
    BenevolentPlayerStrategy(BenevolentPlayerStrategy& other);  // Parameterized Constructor

    BenevolentPlayerStrategy& operator = (const BenevolentPlayerStrategy& other); // Assignment operator overloading
    friend std::ostream& operator << (std::ostream& out, const BenevolentPlayerStrategy& source); // Input stream operator

    Order* issueOrder(GameEngine& gameEngine) override; // Overriding virtual function issueOrder() from base class PlayerStrategy
    vector<Territory*> toAttack(GameEngine& gameEngine) override;   // Overriding virtual function toAttack() from base class PlayerStrategy
    vector<Territory*> toDefend(GameEngine& gameEngine) override;   // Overriding virtual function toDefend() from base class PlayerStrategy
};



//========== Aggressive PlayerStrategy Class ==========//

class AggressivePlayerStrategy : public PlayerStrategy
{
public:

    AggressivePlayerStrategy(); // Default Constructor
    AggressivePlayerStrategy(Player& player);   // Parameterized Constructor
    AggressivePlayerStrategy(AggressivePlayerStrategy& other);  // Parameterized Constructor

    AggressivePlayerStrategy& operator = (const AggressivePlayerStrategy& other); // Assignment operator overloading
    friend std::ostream& operator << (std::ostream& out, const AggressivePlayerStrategy& source); // Input stream operator

    Order* issueOrder(GameEngine& gameEngine) override; // Overriding virtual function issueOrder() from base class PlayerStrategy
    vector<Territory*> toAttack(GameEngine& gameEngine) override;   // Overriding virtual function toAttack() from base class PlayerStrategy
    vector<Territory*> toDefend(GameEngine& gameEngine) override;   // Overriding virtual function toDefend() from base class PlayerStrategy
};



//========== Cheater PlayerStrategy Class ==========//

class CheaterPlayerStrategy : public PlayerStrategy
{
public:

    CheaterPlayerStrategy();    // Default Constructor
    CheaterPlayerStrategy(Player& player);  // Parameterized Constructor
    CheaterPlayerStrategy(CheaterPlayerStrategy& other);    // Parameterized Constructor

    CheaterPlayerStrategy& operator = (const CheaterPlayerStrategy& other); // Assignment operator overloading
    friend std::ostream& operator << (std::ostream& out, const CheaterPlayerStrategy& source); // Input stream operator

    Order* issueOrder(GameEngine& gameEngine) override; // Overriding virtual function issueOrder() from base class PlayerStrategy
    vector<Territory*> toAttack(GameEngine& gameEngine) override;   // Overriding virtual function toAttack() from base class PlayerStrategy
    vector<Territory*> toDefend(GameEngine& gameEngine) override;   // Overriding virtual function toDefend() from base class PlayerStrategy
};



//========== Neutral PlayerStrategy Class ==========//

class NeutralPlayerStrategy : public PlayerStrategy
{
public:


};