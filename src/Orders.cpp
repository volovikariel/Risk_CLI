#include <iostream>
#include "Orders.h"

using namespace std;

//==================== Order Class ====================

//Default Constructor
Order::Order() {
    this->orderName = "Order";
}

//Copy Constructor
Order::Order(const Order& copy) {
    this->orderName = copy.orderName;
}

//Parameterized Constructor
Order::Order(const string &orderName) {
    this->orderName = orderName;
}

//Getter for order name
string Order::getName() {
    return this->orderName;
}

//Setter for order name
void Order::setName(const string& name) {
    this->orderName = name;
}

//Destructor
Order::~Order() { }

//Input stream operator
ostream& operator<<(ostream& os, Order& order){
    return os << "Order type: " << order.getName() << endl;

    //TODO check if executed, then print description
}

//Assignment Operator
Order& Order::operator=(const Order& order) {
    this->orderName = order.orderName;
    return *this;
}









//==================== OrderList Class ====================

//Default Constructor
OrdersList::OrdersList() { }

//Copy Constructor
OrdersList::OrdersList(const OrdersList& ordersList):
    orderList(ordersList.orderList)
{

}

//Parameterized Constructor
OrdersList::OrdersList(vector<Order*>& orderList):
    orderList(orderList)
{

}

//Destructor
OrdersList::~OrdersList() {
    for(Order* o : orderList){
        delete o;
    }
}

//Removes an order from the specified index
void OrdersList::removeOrder(int index) {
    Order* order = orderList.at(index);
    orderList.erase(orderList.begin() + index);
    cout << "Deleted Order at index: " << index << endl << endl;
    delete order;
}

//Move the order from index x (from) to index y (to)
void OrdersList::move(int from, int to) {
    //Save the element at index from as temp
    Order *temporaryOrder = orderList[from];
    //Remove the element at that the from index
    orderList.erase(orderList.begin() + from);
    //Insert the element that was at the from index (now saved in temp)
    orderList.insert(orderList.begin() + to, temporaryOrder);
    cout << "Moved order from index " << from << " to the index " << to << "." << endl << endl;
}

//Add an order to the orders list
void OrdersList::addOrder(Order *order) {
    orderList.push_back(order);
}

//Assignment operator
OrdersList &OrdersList::operator=(const OrdersList &ordersList) {
    for (const Order* order : orderList) {
        delete order;
    }
    orderList = ordersList.orderList;
    return *this;
}

//Stream insertion
ostream &operator<<(ostream &os, OrdersList &ordersList) {
    for(int i = 0; i < ordersList.orderList.size(); i++){
        os << "[Order " + to_string(i) << "] " << ordersList.getOrdersList()[i]->getName() << endl;
    }
    return os;
}

//Getter for orderList
vector<Order *>& OrdersList::getOrdersList() {
    return orderList;
}




//==================== Deploy Class ====================

//Default Constructor
Deploy::Deploy() {
    this->setName("Deploy");
}

//Copy Constructor
Deploy::Deploy(const Deploy& deploy) : Order(deploy) {
    this->orderName = deploy.orderName;
}

//Destructor
Deploy::~Deploy() {
    cout << "Destroying order: deploy." << endl;
}

//Execute : First validates the order, and if valid executes its action
bool Deploy::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

//Validate : checks if an order is valid
bool Deploy::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

//Assignment operator
Deploy &Deploy::operator=(const Deploy &order) {
    Order::operator=(order);
    this->orderName = order.orderName;
    return *this;
}

ostream &operator<<(ostream &os, Deploy &deploy) {
    return os << "Order name: " << deploy.getName();
}






// ==================== Advance Class ====================

Advance::Advance() {
    this->setName("Advance");
}

Advance::Advance(const Advance &advance) : Order(advance) {
    this->orderName = advance.orderName;
}

Advance::~Advance() {
    cout << "Destroying order: advance." << endl;
}

bool Advance::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

bool Advance::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

Advance &Advance::operator=(const Advance &advance) {
    Order::operator=(advance);
    this->orderName = advance.orderName;
    return *this;
}

ostream &operator<<(ostream &os, Advance &advance) {
    return os << "Order name: " << advance.getName();
}







// ==================== Bomb Class ====================


Bomb::Bomb() {
    this->setName("Bomb");
}

Bomb::Bomb(const Bomb &bomb) : Order(bomb) {
    this->orderName = bomb.orderName;
}

Bomb::~Bomb() {
    cout << "Destroying order: bomb." << endl;
}

bool Bomb::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

bool Bomb::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

Bomb &Bomb::operator=(const Bomb &bomb) {
    Order::operator=(bomb);
    this->orderName = bomb.orderName;
    return *this;
}

ostream &operator<<(ostream &os, Bomb &bomb) {
    return os << "Order name: " << bomb.getName();
}




// ==================== Blockade Class ====================


Blockade::Blockade() {
    this->setName("Blockade");
}

Blockade::Blockade(const Blockade &blockade) : Order(blockade){
    this->orderName = blockade.orderName;
}

Blockade::~Blockade() {
    cout << "Destroying order: blockade." << endl;
}

bool Blockade::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

bool Blockade::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

Blockade &Blockade::operator=(const Blockade &blockade) {
    Order::operator=(blockade);
    this->orderName = blockade.orderName;
    return *this;
}

ostream &operator<<(ostream &os, Blockade &blockade) {
    return os << "Order name: " << blockade.getName();
}






// ==================== Airlift Class ====================


Airlift::Airlift() {
    this->setName("Airlift");
}

Airlift::Airlift(const Airlift &airlift) : Order(airlift){
    this->orderName = airlift.orderName;
}

Airlift::~Airlift() {
    cout << "Destroying order: airlift." << endl;
}

bool Airlift::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

bool Airlift::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

Airlift &Airlift::operator=(const Airlift &airlift) {
    Order::operator=(airlift);
    this->orderName = airlift.orderName;
    return *this;
}

ostream &operator<<(ostream &os, Airlift &airlift) {
    return os << "Order name: " << airlift.getName();
}










// ==================== Negotiate Class ====================


Negotiate::Negotiate() {
    this->setName("Negotiate");
}

Negotiate::Negotiate(const Negotiate &negotiate) : Order(negotiate){
    this->orderName = negotiate.orderName;
}

Negotiate::~Negotiate() {
    cout << "Destroying order: airlift." << endl;
}

bool Negotiate::execute() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

bool Negotiate::validate() {
    //----- NO DESCRIPTION TO IMPLEMENT THIS FOR A1 -----
    return false;
}

Negotiate &Negotiate::operator=(const Negotiate &negotiate) {
    Order::operator=(negotiate);
    this->orderName = negotiate.orderName;
    return *this;
}

ostream &operator<<(ostream &os, Negotiate &negotiate) {
    return os << "Order name: " << negotiate.getName();
}




