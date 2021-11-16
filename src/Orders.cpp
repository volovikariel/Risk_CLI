#include "Orders.h"
#include "Map.h"
#include "Player.h"

#include <algorithm>
#include <sstream>

//==================== Order Class ====================

// Stream output operator of Order_Type
std::ostream& operator << (std::ostream& out, const Order::Type source)
{
    // Names for the orders
    static const char* names[9] =
    {
        "Order",
        "Deploy",
        "Advance",
        "Bomb",
        "Blockade",
        "Airlift",
        "Negotiate"
    };

    out << names[static_cast<size_t>(source)];
    return out;
}

// Default Constructor
Order::Order()
{
    this->orderType = Type::Order;
}

// Copy Constructor
Order::Order(const Order& copy):
    orderType(copy.orderType)
{
}

// Parameterized Constructor
Order::Order(const Type orderType):
    orderType(orderType)
{

}

// Getter for the order type
Order::Type Order::getType() const
{
    return orderType;
}

// Setter for the order type
void Order::setType(Type orderType)
{
    this->orderType = orderType;
}

// Setter the execution effect
void Order::saveEffect(const std::string& effect)
{
    this->effect = effect;
    // Notify the observers that the Command object has changed
    notify();
}

// Destructor
Order::~Order()
{

}

// Input stream operator
ostream& operator << (ostream& out, const Order& order)
{
    return order.print(out);
}

// Assignment Operator
Order& Order::operator = (const Order& order)
{
    this->orderType = order.orderType;
    return *this;
}

// Execute method (Pure virtual)
bool Order::execute()
{
    // TODO: How will it get this info though, how does it know if we're about to execute?
    // Maybe set executed -> true
    // Notify the observers that an Order was executed
    notify();
    return true;
}

// Validate method
bool Order::validate()
{
    return true;
}

//Print the order type
ostream& Order::print(ostream& out) const
{
    return out << "Order type: " << getType() << endl;
}

// Getter for bool executed
bool Order::getExecuted() const
{
    return this->executed;
}

// Setter for bool executed
void Order::setExecuted(bool value)
{
    this->executed = value;
}

string Order::stringToLog()
{
    //TODO: Return a Command specific string
    return "Order: UNFINISHED\n";
}



//==================== OrderList Class ====================

//Default Constructor
OrdersList::OrdersList():
    orderList()
{

}

// Copy Constructor
OrdersList::OrdersList(const OrdersList& other)
{
    deepCopy(other.orderList);
}

// Parameterized Constructor
OrdersList::OrdersList(const vector<Order*>& orderList)
{
    deepCopy(orderList);
}

// Destructor
OrdersList::~OrdersList()
{
    for(Order* order : orderList)
    {
        delete order;
    }
}

// Removes an order from the specified index
void OrdersList::remove(int index)
{
    Order* order = orderList.at(index);
    orderList.erase(orderList.begin() + index);
    cout << "Deleted Order at index: " << index << endl << endl;
    delete order;
}

// Move the order from index x (from) to index y (to)
void OrdersList::move(int from, int to)
{
    std::swap(orderList[from], orderList[to]);
    cout << "Moved order from index " << from << " to the index " << to << "." << endl << endl;
}

// Add an order to the orders list
void OrdersList::addOrder(Order *order)
{
    orderList.push_back(order);
    cout << "Added Order " << order->getType() << endl << endl;
    // Notify the observers that an order of type etc etc was added to an orderList
    notify();
}

// Assignment operator
OrdersList& OrdersList::operator = (const OrdersList& other)
{
    for (const Order* order : orderList)
    {
        delete order;
    }

    deepCopy(other.orderList);

    return *this;
}

// Stream insertion operator
ostream& operator << (ostream& out, const OrdersList& source)
{
    for (size_t i = 0; i < source.orderList.size(); i++)
    {
        out << "[Order " + to_string(i) << "] " << source.getOrdersList()[i]->getType() << endl;
    }
    return out;
}

// Getter for orderList
vector<Order *>& OrdersList::getOrdersList()
{
    return orderList;
}

// Const getter for orderList
const vector<Order *>& OrdersList::getOrdersList() const
{
    return orderList;
}

// Method to deep copy the order list
void OrdersList::deepCopy(const vector<Order*>& orderList)
{
    for (const Order* order : orderList)
    {
        switch (order->getType())
        {
            case (Order::Type::Deploy):
            {
                this->orderList.push_back(new Deploy(*static_cast<const Deploy*>(order)));
                break;
            }
            case (Order::Type::Advance):
            {
                this->orderList.push_back(new Advance(*static_cast<const Advance*>(order)));
                break;
            }
            case (Order::Type::Bomb):
            {
                this->orderList.push_back(new Bomb(*static_cast<const Bomb*>(order)));
                break;
            }
            case (Order::Type::Blockade):
            {
                this->orderList.push_back(new Blockade(*static_cast<const Blockade*>(order)));
                break;
            }
            case (Order::Type::Airlift):
            {
                this->orderList.push_back(new Airlift(*static_cast<const Airlift*>(order)));
                break;
            }
            case (Order::Type::Negotiate):
            {
                this->orderList.push_back(new Negotiate(*static_cast<const Negotiate*>(order)));
                break;
            }
        }
    }
}

string OrdersList::stringToLog()
{
    // These are the types to which the Type enum maps to
    string types[] =
    {
        "Order",
        "Deploy",
        "Advance",
        "Bomb",
        "Blockade",
        "Airlift",
        "Negotiate"
    };

    Order* most_recently_added_order = this->getOrdersList()[this->getOrdersList().size() - 1];
    return "OrdersList: Last order added to the list had a type of '" + types[(int)most_recently_added_order->getType()] + "'\n";
}



//==================== Deploy Class ====================

// Default Constructor
Deploy::Deploy()
{
    this->setType(Type::Deploy);
    this->setExecuted(true);
}

// Copy Constructor
Deploy::Deploy(const Deploy& source):
    Order(source)
{
    this->setType(Type::Deploy);
}

// Parameterized Constructor
Deploy::Deploy(int armies, Player& player, Territory& territory):
    Order(),
    armies(armies),
    player(&player),
    territory(&territory)
{
    this->setType(Type::Deploy);
}

// Destructor
Deploy::~Deploy()
{

}

// Execute : First validates the order, and if valid executes its action
bool Deploy::execute()
{
    if (validate())
    {
        player->setPlayerArmies(player->getPlayerArmies() - armies);
        territory->armies += armies;

        std::ostringstream stream;
        stream << "Deployed " << armies << " armies on territory " << territory->name << " for player " << player->getPlayerName();
        saveEffect(stream.str());

        return true;
    }
    else
    {
        return false;
    }
}

// Validate : checks if an order is valid
bool Deploy::validate()
{
    if (player == nullptr || territory == nullptr)
    {
        saveEffect("nullptr arguments passed");
    }
    else if (armies <= 0)
    {
        saveEffect("Not enough armies");
    }
    else if (!player->hasTerritory(territory))
    {
        saveEffect("Territory doesn't belong to player");
    }
    else
    {
        return true;
    }

    return false;
}

// Assignment operator
Deploy& Deploy::operator = (const Deploy& other)
{
    Order::operator = (other);
    return *this;
}

// Stream operator that calls the print
ostream& operator<<(ostream& out, const Deploy& source)
{
    return source.print(out);
}

// Print method to display the description and effect of the order
ostream& Deploy::print(ostream& out) const
{
    out << "[Deploy Description]: Places some armies on one of the player's territories." << endl;
    if (executed)
    {
        out << "[Deploy Effect]: " << effect << endl;
    }
    return out;
}




// ==================== Advance Class ====================

// Default Constructor
Advance::Advance()
{
    this->setType(Type::Advance);
}

// Copy Constructor
Advance::Advance(const Advance &other):
    Order(other)
{
    this->setType(Type::Advance);
}

// Parameterized Constructor
Advance::Advance(int armies, Player& player, Territory& sourceTerritory, Territory& targetTerritory, Map& map):
    Order(),
    armies(armies),
    player(&player),
    sourceTerritory(&sourceTerritory),
    targetTerritory(&targetTerritory),
    map(&map)
{
    this->setType(Type::Advance);
}

// Destructor
Advance::~Advance()
{

}

float random01()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

//Execute : First validates the order, and if valid executes its action
bool Advance::execute()
{
    if (validate())
    {
        // If the source and target territories both belong to the player, then we just move the armies there
        if (player->hasTerritory(sourceTerritory) && player->hasTerritory(targetTerritory))
        {
            sourceTerritory->armies -= this->armies; // Move the number of armies to the target territory
            targetTerritory->armies += this->armies;

            std::ostringstream stream;
            stream << "Advanced " << armies << " armies from territory " << sourceTerritory->name << " to territory " << targetTerritory->name << " for player " << player->getPlayerName();
            saveEffect(stream.str());
        }
        // If the target territory does not belong to the player, then we attack.
        else if (!player->hasTerritory(targetTerritory))
        {
            int armiesAttacking = armies;
            int armiesDefending = targetTerritory->armies;

            int armiesDefendingKilled = 0;
            for (int i = 0; i < armiesAttacking; ++i)
            {
                if (random01() <= 0.6f)
                {
                    armiesDefendingKilled++;
                }
            }

            int armiesAttackingKilled = 0;
            for (int i = 0; i < armiesDefending; ++i)
            {
                if (random01() <= 0.7f)
                {
                    armiesAttackingKilled++;
                }
            }

            int armiesAttackingLeft = max(0, armiesAttacking - armiesAttackingKilled);
            int armiesDefendingLeft = max(0, armiesDefending - armiesDefendingKilled);

            if (armiesDefendingLeft <= 0)
            {
                sourceTerritory->armies -= armiesAttacking;
                targetTerritory->armies = armiesAttackingLeft;

                player->addPlayerTerritory(targetTerritory);
                targetTerritory->player = player;

                std::ostringstream stream;
                stream << "Player " << player->getPlayerName() << " attacked from territory " << sourceTerritory->name << " to " << targetTerritory->name <<
                    " with " << armies << " armies. The territory was conquered. " << armiesAttackingLeft << " attacking armies are left and have occupied the territory.";
                saveEffect(stream.str());

                // TODO: PLAYER MUST GET A CARD SINCE THEY WON.
            }
            else
            {
                sourceTerritory->armies -= armiesAttackingKilled;
                targetTerritory->armies = armiesDefendingLeft;

                std::ostringstream stream;
                stream << "Player " << player->getPlayerName() << " attacked from territory " << sourceTerritory->name << " to " << targetTerritory->name <<
                    " with " << armies << " armies. " << armiesAttackingLeft << " attacking armies are left and " << armiesDefendingLeft << " defending armies are left.";
                saveEffect(stream.str());
            }
        }

        return true;
    }
    else
    {
        return false;
    }
}

//Validate : checks if an order is valid
bool Advance::validate()
{
    if (player == nullptr || sourceTerritory == nullptr || targetTerritory == nullptr)
    {
        saveEffect("nullptr arguments passed");
    }
    else if (armies <= 0)
    {
        saveEffect("Not enough armies");
    }
    else if (std::find(player->getUnattackable().begin(), player->getUnattackable().end(), targetTerritory->player) != player->getUnattackable().end())
    {
        saveEffect("Can't attack diplomatic ally");
    }
    else if (!player->hasTerritory(sourceTerritory))
    {
        saveEffect("The source territory does not belong to the player that issued the order");
    }
    else if (!targetTerritory->isNeighbor(sourceTerritory))
    {
        saveEffect("The target territory is not adjacent to the source territory");
    }
    else
    {
        return true;
    }

    return false;
}

// Assignment operator
Advance& Advance::operator = (const Advance& other)
{
    Order::operator = (other);
    return *this;
}

// Stream operator that calls the print
ostream& operator<<(ostream& out, const Advance& source)
{
    return source.print(out);
}

// Print method to display the description and effect of the order
ostream& Advance::print(ostream& out) const
{
    out << "[Advance Description]: Move some armies from one of the current player's territories to an adjacent territory." << endl;
    if (executed)
    {
        out << "[Advance Effect]: " << effect << endl;
    }
    return out;
}




// ==================== Bomb Class ====================

// Default Constructor
Bomb::Bomb()
{
    this->setType(Type::Bomb);
    this->setExecuted(true);
}

// Copy Constructor
Bomb::Bomb(const Bomb& other):
    Order(other)
{
    this->setType(Type::Bomb);
}

// Parameterized Constructor
Bomb::Bomb(Player& player, Territory& territory):
    Order(),
    player(&player),
    territory(&territory)
{
    this->setType(Type::Bomb);
}

// Destructor
Bomb::~Bomb()
{

}

// Execute : First validates the order, and if valid executes its action
bool Bomb::execute()
{
    if (validate())
    {
        int initialArmies = territory->armies;
        territory->armies /= 2;

        std::ostringstream stream;
        stream << "Player " << player->getPlayerName() << " bombed territory " << territory->name << ". It had " << initialArmies << " armies, now " << territory->armies << " are left.";
        saveEffect(stream.str());

        return true;
    }
    else
    {
        return false;
    }
}

// Validate : checks if an order is valid
bool Bomb::validate()
{
    if (player == nullptr || territory == nullptr)
    {
        saveEffect("nullptr arguments passed");
    }
    else if (player == territory->player)
    {
        saveEffect("Can't bomb own territory");
    }
    else if (std::find(player->getUnattackable().begin(), player->getUnattackable().end(), territory->player) != player->getUnattackable().end())
    {
        saveEffect("Can't bomb a diplomatic ally");
    }
    else
    {
        return true;
    }

    return false;
}

// Assignment operator
Bomb& Bomb::operator = (const Bomb& other)
{
    Order::operator = (other);
    return *this;
}

// Stream operator that calls the print
ostream& operator << (ostream& out, const Bomb& source)
{
    return source.print(out);
}

// Print method to display the description and effect of the order
ostream& Bomb::print(ostream& out) const
{
    out << "[Bomb Description]: Destroy half of the armies on an opponent's territory." << endl;
    if (executed)
    {
        out << "[Bomb Effect]: " << effect << endl;
    }
    return out;
}




// ==================== Blockade Class ====================

// Default Constructor
Blockade::Blockade()
{
    this->setType(Type::Blockade);
}

// Copy Constructor
Blockade::Blockade(const Blockade& other):
    Order(other)
{
    this->setType(Type::Blockade);
}

// Parameterized Constructor
Blockade::Blockade(Player& player, Player& neutralPlayer, Territory& territory):
    Order(),
    player(&player),
    neutralPlayer(&neutralPlayer),
    territory(&territory)
{
    this->setType(Type::Blockade);
}

// Destructor
Blockade::~Blockade()
{

}

// Execute : First validates the order, and if valid executes its action
bool Blockade::execute()
{
    if (validate())
    {
        int initialArmies = territory->armies;
        territory->armies *= 2;
        territory->player = neutralPlayer;

        std::ostringstream stream;
        stream << "Player " << player->getPlayerName() << " blockaded territory " << territory->name << ". It had " << initialArmies << " armies, now it has " << territory->armies << ".";
        saveEffect(stream.str());

        return true;
    }
    else
    {
        return false;
    }
}

// Validate : checks if an order is valid
bool Blockade::validate()
{
    if (player == nullptr || neutralPlayer == nullptr || territory == nullptr)
    {
        saveEffect("nullptr arguments passed");
    }
    else if (player != territory->player)
    {
        saveEffect("Can't blockade another player's territory");
    }
    else
    {
        return true;
    }

    return false;
}

// Assignment operator
Blockade& Blockade::operator = (const Blockade& other)
{
    Order::operator = (other);
    return *this;
}

// Stream operator that calls the print
ostream& operator << (ostream& out, const Blockade& source)
{
    return source.print(out);
}

// Print method to display the description and effect of the order
ostream &Blockade::print(ostream& out) const
{
    out << "[Blockade Description]: Double the number of armies on one of the current player's territories and make it a neutral territory." << endl;
    if (executed)
    {
        out << "[Blockade Effect]: " << effect << endl;
    }
    return out;
}




// ==================== Airlift Class ====================

// Default Constructor
Airlift::Airlift()
{
    this->setType(Type::Airlift);
}

// Copy Constructor
Airlift::Airlift(const Airlift& other):
    Order(other)
{
    this->setType(Type::Airlift);
}

//Parameterized Constructor
Airlift::Airlift(const int &playerArmies, Player *const player, Territory *source, Territory *target) : Order() {
    this->armies = playerArmies;
    this->player = player;
    this->sourceTerritory = source;
    this->targetTerritory = target;
}

// Destructor
Airlift::~Airlift()
{
    cout << "Destroying order: airlift." << endl;
}

// Execute : First validates the order, and if valid executes its action
bool Airlift::execute()
{
    if(validate()){
        //TODO: THE AIRLIFT IS ONLY FOR THE PLAYER WHO OWNS THE SOURCE AND TARGET TERRITORIES RIGHT? DOESN'T PRODUCE AN ATTACK IF ITS NOT THEIRS
        //TODO: MAKE SURE THAT THIS IS ALL THAT NEEDS TO BE DONE HERE
        sourceTerritory->armies -= this->armies;
        targetTerritory->armies += armies;

        cout << "[AirLift] Airlift order executed, armies were moved from source to target territory." << endl;
    }
    return false;
}

// Validate : checks if an order is valid
bool Airlift::validate()
{
    if(player->hasTerritory(sourceTerritory) && player->hasTerritory(targetTerritory)){
        return true;
    }
    else {
        cout << "[Airlift] Invalid airlift order. Source or target territory does not belong to player." << endl;
        return false;
    }
    return false;
}

// Assignment operator
Airlift &Airlift::operator = (const Airlift& other)
{
    Order::operator = (other);
    return *this;
}

// Stream operator that calls the print
ostream& operator << (ostream& out, const Airlift& source)
{
    return source.print(out);
}

// Print method to display the description and effect of the order
ostream& Airlift::print(ostream& out) const
{
    Order::print(out);
    out << "[Airlift Description]: Advance some armies from one of the current player’s territories to any another territory." << endl;
    if (executed)
    {
        out << "[Airlift Effect]: Move any number of army units from one of your territories to another territory, even if they are not adjacent." << endl;
    }
    return out;
}




// ==================== Negotiate Class ====================

// Default Constructor
Negotiate::Negotiate()
{
    this->setType(Type::Negotiate);
}

// Copy Constructor
Negotiate::Negotiate(const Negotiate& other):
    Order(other)
{
    this->setType(Type::Negotiate);
}

// Parameterized Constructor
Negotiate::Negotiate(Player *player, Player *target):
    Order()
{
    this->setType(Type::Blockade);
    this->player = player;
    this->target = target;
}

// Destructor
Negotiate::~Negotiate()
{
    cout << "Destroying order: airlift." << endl;
}

// Execute : First validates the order, and if valid executes its action
bool Negotiate::execute()
{
    cout << "[Negotiate] Inside execute()" << endl;
    if(validate()){
        player->setUnattackable(target);
        target->setUnattackable(player);
        cout << "[Negotiate] valid order executed." << endl;
        return true;
    }else{
        return false;
    }
}

// Validate : checks if an order is valid
bool Negotiate::validate()
{
    cout << "[Negotiate] Inside validate()" << endl;
    if (target->getPlayerID()!=player->getPlayerID()){
        return true;
    }else{
        return false;
    }
}

// Assignment operator
Negotiate& Negotiate::operator = (const Negotiate& other)
{
    Order::operator = (other);
    return *this;
}

// Stream operator that calls the print
ostream& operator << (ostream& out, const Negotiate& source)
{
    return source.print(out);
}

// Print method to display the description and effect of the order
ostream& Negotiate::print(ostream& out) const
{
    Order::print(out);
    out << "[Negotiate Description]: Prevent attacks between the current player and another player until the end of the turn." << endl;
    if (executed)
    {
        out << "[Negotiate Effect]: until the end of the turn, you and the target player cannot attack each other." << endl;
    }
    return out;
}
