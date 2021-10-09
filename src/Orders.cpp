#include <iostream>
#include "Orders.h"

using namespace std;

//==================== Order Class ====================

// Stream output operator of Order_Type
std::ostream& operator << (std::ostream& out, const Order::Order_Type order)
{
    //Names for the orders
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

    out << names[static_cast<size_t>(order)];
    return out;
}

//Default Constructor
Order::Order() {
    this->orderType = Order_Type::order;
}

//Copy Constructor
Order::Order(const Order& copy):
    orderType(copy.orderType)
{
}

//Parameterized Constructor
Order::Order(const Order_Type &orderType):
    orderType(orderType)
{

}

//Getter for the order type
Order::Order_Type Order::getType() const {
    return orderType;
}

// Setter for the order type
void Order::setType(Order_Type orderType) {
    this->orderType = orderType;
}

//Destructor
Order::~Order() { }

//Input stream operator
ostream& operator<<(ostream& os, const Order& order){
    return order.print(os);
}

//Assignment Operator
Order& Order::operator=(const Order& order) {
    this->orderType = order.orderType;
    return *this;
}

//Execute method
bool Order::execute()
{
    return true;
}

//Validate method
bool Order::validate()
{
    return true;
}

//Print the order type
ostream& Order::print(ostream& os) const
{
    return os << "Order type: " << getType() << endl;
}

//Getter for bool executed
bool Order::getExecuted() const {
    return this->executed;
}

//Setter for bool executed
void Order::setExecuted(bool value) {
    this->executed = value;
}









//==================== OrderList Class ====================

//Default Constructor
OrdersList::OrdersList():
    orderList()
{

}

//Copy Constructor
OrdersList::OrdersList(const OrdersList& ordersList)
{
    deepCopy(ordersList.orderList);
}

//Parameterized Constructor
OrdersList::OrdersList(const vector<Order*>& orderList)
{
    deepCopy(orderList);
}

//Destructor
OrdersList::~OrdersList() {
    for(Order* o : orderList){
        delete o;
    }
}

//Removes an order from the specified index
void OrdersList::remove(int index) {
    Order* order = orderList.at(index);
    orderList.erase(orderList.begin() + index);
    cout << "Deleted Order at index: " << index << endl << endl;
    delete order;
}

//Move the order from index x (from) to index y (to)
void OrdersList::move(int from, int to) {
    std::swap(*orderList[from], *orderList[to]);
    cout << "Moved order from index " << from << " to the index " << to << "." << endl << endl;
}

//Add an order to the orders list
void OrdersList::addOrder(Order *order) {
    orderList.push_back(order);
    cout << "Added Order " << order->getType() << endl << endl;
}

//Assignment operator
OrdersList &OrdersList::operator=(const OrdersList &ordersList) {
    for (const Order* order : orderList) {
        delete order;
    }
    deepCopy(ordersList.orderList);
    return *this;
}

//Stream insertion
ostream &operator<<(ostream &os, const OrdersList &ordersList) {
    for(int i = 0; i < ordersList.orderList.size(); i++){
        os << "[Order " + to_string(i) << "] " << ordersList.getOrdersList()[i]->getType() << endl;
    }
    return os;
}

//Getter for orderList
vector<Order *>& OrdersList::getOrdersList() {
    return orderList;
}

//Const getter for orderList
const vector<Order *>& OrdersList::getOrdersList() const {
    return orderList;
}

//Method to deep copy the order list
void OrdersList::deepCopy(const vector<Order*>& orderList)
{
    for(const Order* order : orderList){
        switch (order->getType()) {
            case (Order::Order_Type::order):
            {
                this->orderList.push_back(new Order(*order));
                break;
            }
            case (Order::Order_Type::deploy):
            {
                this->orderList.push_back(new Deploy(*static_cast<const Deploy*>(order)));
                break;
            }
            case (Order::Order_Type::advance):
            {
                this->orderList.push_back(new Advance(*static_cast<const Advance*>(order)));
                break;
            }
            case (Order::Order_Type::bomb):
            {
                this->orderList.push_back(new Bomb(*static_cast<const Bomb*>(order)));
                break;
            }
            case (Order::Order_Type::blockade):
            {
                this->orderList.push_back(new Blockade(*static_cast<const Blockade*>(order)));
                break;
            }
            case (Order::Order_Type::airlift):
            {
                this->orderList.push_back(new Airlift(*static_cast<const Airlift*>(order)));
                break;
            }
            case (Order::Order_Type::negotiate):
            {
                this->orderList.push_back(new Negotiate(*static_cast<const Negotiate*>(order)));
                break;
            }
        }
    }
}




//==================== Deploy Class ====================

//Default Constructor
Deploy::Deploy() {
    this->setType(Order_Type::deploy);
    this->setExecuted(true);
}

//Copy Constructor
Deploy::Deploy(const Deploy& deploy) : Order(deploy) {
    this->setType(Order_Type::deploy);
}

//Destructor
Deploy::~Deploy() {
    cout << "Destroying order: deploy." << endl;
}

//Execute : First validates the order, and if valid executes its action
bool Deploy::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Deploy] Inside execute()" << endl;
//    if(validate()){
//        //TO BE IMPLEMENTED LATER
//    }
    return false;
}

//Validate : checks if an order is valid
bool Deploy::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Deploy] Inside validate()" << endl;
    return false;
}

//Assignment operator
Deploy &Deploy::operator=(const Deploy &order) {
    Order::operator=(order);
    return *this;
}

//Stream operator that calls the print
ostream &operator<<(ostream &os, const Deploy &deploy) {
    return deploy.print(os);
}

//Print method to display the description and effect of the order
ostream& Deploy::print(ostream& os) const {
    Order::print(os);
    os << "[Deploy Description]: Places some armies on one of the current player's territories" << endl;
    if (executed) {
        os << "[Deploy Effect]: Places armies on territories" << endl;
    }
    return os;
}






// ==================== Advance Class ====================

//Default Constructor
Advance::Advance() {
    this->setType(Order_Type::advance);
}

//Copy Constructor
Advance::Advance(const Advance &advance) : Order(advance) {
    this->setType(Order_Type::advance);
}

//Destructor
Advance::~Advance() {
    cout << "Destroying order: advance." << endl;
}

//Execute : First validates the order, and if valid executes its action
bool Advance::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Advance] Inside execute()" << endl;
//    if(validate()){
//
//    }
    return false;
}

//Validate : checks if an order is valid
bool Advance::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Advance] Inside validate()" << endl;
    return false;
}

//Assignment operator
Advance &Advance::operator=(const Advance &advance) {
    Order::operator=(advance);
    return *this;
}

//Stream operator that calls the print
ostream &operator<<(ostream &os, const Advance &advance) {
    return advance.print(os);
}

//Print method to display the description and effect of the order
ostream &Advance::print(ostream &os) const {
    Order::print(os);
    os << "[Advance Description]: Move some armies from one of the current player's territories to an adjacent territory." << endl;
    if (executed) {
        os << "[Advance Effect]: If the territory belongs to the player, the armies move there. If it belongs to an other player, an attack happens." << endl;
    }
    return os;
}










// ==================== Bomb Class ====================

//Default Constructor
Bomb::Bomb() {
    this->setType(Order_Type::bomb);
    this->setExecuted(true);
}

//Copy Constructor
Bomb::Bomb(const Bomb &bomb) : Order(bomb) {
    this->setType(Order_Type::bomb);
}

//Destructor
Bomb::~Bomb() {
    cout << "Destroying order: bomb." << endl;
}

//Execute : First validates the order, and if valid executes its action
bool Bomb::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Bomb] Inside execute()" << endl;
//    if(validate()){
//
//    }
    return false;
}

//Validate : checks if an order is valid
bool Bomb::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Bomb] Inside validate()" << endl;
    return false;
}

//Assignment operator
Bomb &Bomb::operator=(const Bomb &bomb) {
    Order::operator=(bomb);
    return *this;
}

//Stream operator that calls the print
ostream &operator<<(ostream &os, const Bomb &bomb) {
    return bomb.print(os);
}


//Print method to display the description and effect of the order
ostream &Bomb::print(ostream &os) const {
    Order::print(os);
    os << "[Bomb Description]: Destroy half of the armies on opponent's territory which is adjacent to one of the current player's territory." << endl;
    if (executed) {
        os << "[Bomb Effect]: Target country loses half of their army units." << endl;
    }
    return os;
}




// ==================== Blockade Class ====================

//Default Constructor
Blockade::Blockade() {
    this->setType(Order_Type::blockade);
}

//Copy Constructor
Blockade::Blockade(const Blockade &blockade) : Order(blockade){
    this->setType(Order_Type::blockade);
}

//Destructor
Blockade::~Blockade() {
    cout << "Destroying order: blockade." << endl;
}

//Execute : First validates the order, and if valid executes its action
bool Blockade::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Blockade] Inside execute()" << endl;
//    if(validate()){
//
//    }
    return false;
}

//Validate : checks if an order is valid
bool Blockade::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Blockade] Inside validate()" << endl;
    return false;
}

//Assignment operator
Blockade &Blockade::operator=(const Blockade &blockade) {
    Order::operator=(blockade);
    return *this;
}

//Stream operator that calls the print
ostream &operator<<(ostream &os, const Blockade &blockade) {
    return blockade.print(os);
}

//Print method to display the description and effect of the order
ostream &Blockade::print(ostream &os) const {
    Order::print(os);
    os << "[Blockade Description]: Triple the number of armies on one of the current player’s territories and make it a neutral territory." << endl;
    if (executed) {
        os << "[Blockade Effect]: the target territory’s army units count is tripled, and the territory becomes neutral." << endl;
    }
    return os;
}






// ==================== Airlift Class ====================

//Default Constructor
Airlift::Airlift() {
    this->setType(Order_Type::airlift);
}

//Copy Constructor
Airlift::Airlift(const Airlift &airlift) : Order(airlift){
    this->setType(Order_Type::airlift);
}

//Destructor
Airlift::~Airlift() {
    cout << "Destroying order: airlift." << endl;
}

//Execute : First validates the order, and if valid executes its action
bool Airlift::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Airlift] Inside execute()" << endl;
//    if(validate()){
//
//    }
    return false;
}

//Validate : checks if an order is valid
bool Airlift::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Airlift] Inside validate()" << endl;
    return false;
}

//Assignment operator
Airlift &Airlift::operator=(const Airlift &airlift) {
    Order::operator=(airlift);
    return *this;
}

//Stream operator that calls the print
ostream &operator<<(ostream &os, const Airlift &airlift) {
    return airlift.print(os);
}

//Print method to display the description and effect of the order
ostream &Airlift::print(ostream &os) const {
    Order::print(os);
    os << "[Airlift Description]: Advance some armies from one of the current player’s territories to any another territory." << endl;
    if (executed) {
        os << "[Airlift Effect]: Move any number of army units from one of your territories to another territory, even if they are not adjacent." << endl;
    }
    return os;
}







// ==================== Negotiate Class ====================

//Default Constructor
Negotiate::Negotiate() {
    this->setType(Order_Type::negotiate);
}

//Copy Constructor
Negotiate::Negotiate(const Negotiate &negotiate) : Order(negotiate){
    this->setType(Order_Type::negotiate);
}

//Destructor
Negotiate::~Negotiate() {
    cout << "Destroying order: airlift." << endl;
}

//Execute : First validates the order, and if valid executes its action
bool Negotiate::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Negotiate] Inside execute()" << endl;
//    if(validate()){
//
//    }
    return false;
}

//Validate : checks if an order is valid
bool Negotiate::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    cout << "[Negotiate] Inside validate()" << endl;
    return false;
}

//Assignment operator
Negotiate &Negotiate::operator=(const Negotiate &negotiate) {
    Order::operator=(negotiate);
    return *this;
}

//Stream operator that calls the print
ostream &operator<<(ostream &os, const Negotiate &negotiate) {
    return negotiate.print(os);
}

//Print method to display the description and effect of the order
ostream &Negotiate::print(ostream &os) const {
    Order::print(os);
    os << "[Negotiate Description]: Prevent attacks between the current player and another player until the end of the turn." << endl;
    if (executed) {
        os << "[Negotiate Effect]: until the end of the turn, you and the target player cannot attack each other." << endl;
    }
    return os;
}




