#include "Orders.h"
#include "Map.h"
#include "Player.h"

#include <algorithm>

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
Deploy::Deploy(const int &playerArmies, Player *player, Territory *targetTerritory) : Order() {
    this->setType(Type::Deploy);
    this->playerID = player->getPlayerID();
    this->player = player;
    this->armies = playerArmies;
    this->targetTerritory = targetTerritory;
}

// Destructor
Deploy::~Deploy()
{
    cout << "Destroying order: deploy." << endl;
}

// Execute : First validates the order, and if valid executes its action
bool Deploy::execute()
{
    if(validate()){
        player->setPlayerArmies(player->getPlayerArmies() - this->armies);
        targetTerritory->armies += this->armies;

        cout << "[Deploy] valid order executed." << endl;
        return true;
    } else {
        cout << "[Deploy] invalid order." << endl;
        return false;
    }
}

// Validate : checks if an order is valid
bool Deploy::validate()
{
    cout << "[Deploy] Validating Order..." << endl;

    if(targetTerritory != nullptr && armies >= 0 && player->hasTerritory(targetTerritory)){
        return true;
    }
    else {
        return false;
    }
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
    Order::print(out);
    out << "[Deploy Description]: Places some armies on one of the current player's territories" << endl;
    if (executed)
    {
        out << "[Deploy Effect]: Places armies on territories" << endl;
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
Advance::Advance(const int &playerArmies, Player *const player, Territory *source, Territory *target, Map *map,
                 Deck *const deck) :Order() {
    this->armies = playerArmies;
    this->player = player;
    this->sourceTerritory = source;
    this->targetTerritory = target;
    this->map = map;
    this->deck = deck;
}

// Destructor
Advance::~Advance()
{
    cout << "Destroying order: advance." << endl;
}

//Execute : First validates the order, and if valid executes its action
bool Advance::execute() //TODO: NEED TO GO OVER THE LOGIC FOR THIS ORDER
{
    if(validate()){
        //Since the advance is valid, we will remove the specified number of armies from the source territory
        sourceTerritory->armies -= this->armies;

        //If the source and target territories both belong to the player, then we just move the armies there
        if(player->hasTerritory(sourceTerritory) && player->hasTerritory(targetTerritory)){
            targetTerritory->armies += this->armies; //Move the number of armies to the target territory
            cout << "[Advance] Moved armies from source to target territory." << endl;
            return true;
        }

        //If the target territory does not belong to the player, then we attack.
        if(!player->hasTerritory(targetTerritory)){
            cout << "[Advance] Attack has been initiated!" << endl;

            int armiesAttacking = armies;
            int randomNumber = 0;

            //Loop until one of the armies loses
            while(armies > 0 && targetTerritory->armies > 0){
                //Attacking Army: 60% chance of killing the defending army.
                for(int i = 0; i < armies; i++){
                    randomNumber = (rand() % 100) + 1;

                    //If the random number generated is between 1 and 60: we have 60% chance, so we kill 1
                    if(randomNumber >= 60){
                        targetTerritory->armies -= 1;
                    }
                }

                //Defending Army: 70% chance of killing the attacking army.
                for(int i = 0; i < targetTerritory->armies; i++){
                    randomNumber = (rand() % 100) + 1;

                    //If the random number generated is between 1 and 70: we have 70% chance, so we kill 1
                    if(randomNumber >= 70){
                        armies -= 1;
                    }
                }
            }

            //After the attack, 3 cases are possible. 1)Attacker won, 2)Defender won, 3)They both lost

            //CASE 1: Attacker won
            if(targetTerritory->armies <= 0){
                //add the armies that was won
                targetTerritory->armies = armies;
                cout << "[Advance] Advance order was executed, attack was a SUCCESS!" << endl;
                //player won, so add the new territory
                player->addPlayerTerritory(targetTerritory);
                //TODO: PLAYER MUST GET A CARD SINCE THEY WON. IS IT A RANDOM CARD?

                return true;
            }
            //CASE 2: Defender won
            else if(armies <= 0){
                cout << "[Advance] Advance order was executed, but failed to capture the target territory" << endl;
                return false;
            }
            //CASE 3: Both attacker and defender lost
            else if(targetTerritory->armies <= 0 && armies <= 0){
                cout << "[Advance] Advance order was executed, the attack was not successful. " << playerID << " lost their army." << endl;
                return false;
            }
        }

    }
    return false;
}

//Validate : checks if an order is valid
bool Advance::validate()
{
    //If the player is on the unattackable list, then we can't attack.
    if(std::find(player->getUnattackable().begin(), player->getUnattackable().end(), targetTerritory) == player->getUnattackable().end()){
        return false;
    }
    else if(sourceTerritory == nullptr || armies <= 0 || !player->hasTerritory(sourceTerritory)){
        cout << "[Advance] The source territory does not belong to the player that issued the order. Order invalid." << endl;
        return false;
    }
    else if(!targetTerritory->isNeighbor(sourceTerritory)){
        cout << "[Advance] The target territory is not adjacent to the source territory. Order invalid." << endl;
        return false;
    }
    else {
        return true;
    }


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
    Order::print(out);
    out << "[Advance Description]: Move some armies from one of the current player's territories to an adjacent territory." << endl;
    if (executed)
    {
        out << "[Advance Effect]: If the territory belongs to the player, the armies move there. If it belongs to an other player, an attack happens." << endl;
    }
    return out;
}




// ==================== Bomb Class ====================

// Default Constructor
Bomb::Bomb() {
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
Bomb::Bomb(Player *player, Territory *source, Territory *targetTerritory):
    Order() 
{
    this->setType(Type::Bomb);
    this->player = player;
    this->sourceTerritory = source;
    this->targetTerritory = targetTerritory;
}

// Destructor
Bomb::~Bomb()
{
    cout << "Destroying order: bomb." << endl;
}

// Execute : First validates the order, and if valid executes its action
bool Bomb::execute()
{
    cout << "[Bomb] Inside execute()" << endl;
    if(validate()){
        targetTerritory->armies /= 2;
        cout << "[Bomb] valid order executed." << endl;
        return true;
    } else {
        cout << "[Bomb] invalid order." << endl;
        return false;
    }
}

// Validate : checks if an order is valid
bool Bomb::validate()
{
    cout << "[Bomb] Inside validate()" << endl;
     if (targetTerritory != nullptr && player->hasTerritory(sourceTerritory) && !(player->hasTerritory(targetTerritory)) && targetTerritory->isNeighbor(sourceTerritory)
        && std::find(player->getUnattackable().begin(), player->getUnattackable().end(), targetTerritory) == player->getUnattackable().end()){
        return true;
    }else{
        return false;
    }
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
    Order::print(out);
    out << "[Bomb Description]: Destroy half of the armies on opponent's territory which is adjacent to one of the current player's territory." << endl;
    if (executed)
    {
        out << "[Bomb Effect]: Target country loses half of their army units." << endl;
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
Blockade::Blockade(Player *player, Territory *targetTerritory):
    Order()
{
    this->setType(Type::Blockade);
    this->player = player;
    this->targetTerritory = targetTerritory;
}

// Destructor
Blockade::~Blockade()
{
    cout << "Destroying order: blockade." << endl;
}

// Execute : First validates the order, and if valid executes its action
bool Blockade::execute()
{
    cout << "[Blockade] Inside execute()" << endl;
    if(validate()){
        targetTerritory->armies *= 2;
        targetTerritory->player=nullptr;
        cout << "[Blockade] valid order executed." << endl;
        return true;
    }else{
        return false;
    }
}

// Validate : checks if an order is valid
bool Blockade::validate()
{
    cout << "[Blockade] Inside validate()" << endl;
    if(targetTerritory != nullptr && player->hasTerritory(targetTerritory)){
        return true;
    }else{
        return false;
    }
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
    Order::print(out);
    out << "[Blockade Description]: Triple the number of armies on one of the current player’s territories and make it a neutral territory." << endl;
    if (executed)
    {
        out << "[Blockade Effect]: the target territory’s army units count is tripled, and the territory becomes neutral." << endl;
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

// Destructor
Airlift::~Airlift()
{
    cout << "Destroying order: airlift." << endl;
}

// Execute : First validates the order, and if valid executes its action
bool Airlift::execute()
{
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Airlift] Inside execute()" << endl;
//    if(validate()){
//
//    }
    return false;
}

// Validate : checks if an order is valid
bool Airlift::validate()
{
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Airlift] Inside validate()" << endl;
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
