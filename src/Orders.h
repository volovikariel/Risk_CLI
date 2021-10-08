#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ==================== Order Class ====================
class Order {
private:
    string orderName;
public:
    Order();                                                            // Default Constructor
    Order(const Order& copy);                                           // Copy Constructor
    ~Order();                                                           // Destructor
    explicit Order(const string& orderName);                            // Parameterized Constructor to define the type of Order
    string getName();                                                   // Getter for the order name
    void setName(const string& orderName);                              // Setter for the order name
    Order & operator=(const Order& order);                              // Assignment operator overloading
    friend ostream& operator <<(ostream& os, Order& order);             // Input stream operator
};


// ==================== OrderList Class ====================
class OrdersList {
private:
    vector<Order*> orderList;                                           // List (vector) of orders

public:
    OrdersList();                                                       // Default Constructor
    OrdersList(const OrdersList& copy);                                 // Copy Constructor
    explicit OrdersList(vector<Order*>& orderList);
    ~OrdersList();                                                      // Destructor
    void removeOrder(int index);                                        // Removes the order from the list at specified index
    void move(int from, int to);                                        // Moves an order from position x (from) to position y (to)
    void addOrder(Order* order);                                        // Adds an order to the list
    OrdersList& operator =(const OrdersList& orderList);                // Assignment operator overloading
    friend ostream& operator<<(ostream& os, OrdersList &ordersList);    // Stream insertion operator
    vector<Order*>& getOrdersList();                                    // Getter for orders list
};



// ==================== Deploy Class ====================
class Deploy : public Order {
private:
    string orderName;                                                   // Type of the order, in this case Deploy
public:
    Deploy();                                                           // Default Constructor
    Deploy(const Deploy& deploy);                                       // Copy Constructor
    ~Deploy();                                                          // Destructor
    bool execute();                                                     // First validates the order, and if valid executes its action
    bool validate();                                                    // Checks if the order is valid
    Deploy& operator =(const Deploy& deploy);                           // Assignment operator overloading
    friend ostream& operator<<(ostream& os, Deploy &deploy);            // Stream Insertion Operator
};




// ==================== Advance Class ====================
class Advance : public Order {
private:
    string orderName;                                                   // Type of the order, in this case Deploy
public:
    Advance();                                                          // Default Constructor
    Advance(const Advance& advance);                                    // Copy Constructor
    ~Advance();                                                         // Destructor
    bool execute();                                                     // First validates the order, and if valid executes its action
    bool validate();                                                    // Checks if the order is valid
    Advance& operator =(const Advance& advance);                        // Assignment operator overloading
    friend ostream& operator<<(ostream& os, Advance &advance);          // Stream Insertion Operator
};



// ==================== Bomb Class ====================
class Bomb : public Order {
private:
    string orderName;                                                   // Type of the order, in this case Deploy
public:
    Bomb();                                                             // Default Constructor
    Bomb(const Bomb& bomb);                                             // Copy Constructor
    ~Bomb();                                                            // Destructor
    bool execute();                                                     // First validates the order, and if valid executes its action
    bool validate();                                                    // Checks if the order is valid
    Bomb& operator =(const Bomb& bomb);                                 // Assignment operator overloading
    friend ostream& operator<<(ostream& os, Bomb &bomb);                // Stream Insertion Operator
};




// ==================== Blockade Class ====================
class Blockade : public Order {
private:
    string orderName;                                                       // Type of the order, in this case Deploy
public:
    Blockade();                                                             // Default Constructor
    Blockade(const Blockade& blockade);                                     // Copy Constructor
    ~Blockade();                                                            // Destructor
    bool execute();                                                         // First validates the order, and if valid executes its action
    bool validate();                                                        // Checks if the order is valid
    Blockade& operator =(const Blockade& blockade);                         // Assignment operator overloading
    friend ostream& operator<<(ostream& os, Blockade &blockade);            // Stream Insertion Operator
};




// ==================== Airlift Class ====================
class Airlift : public Order {
private:
    string orderName;                                                       // Type of the order, in this case Deploy
public:
    Airlift();                                                              // Default Constructor
    Airlift(const Airlift& airlift);                                        // Copy Constructor
    ~Airlift();                                                             // Destructor
    bool execute();                                                         // First validates the order, and if valid executes its action
    bool validate();                                                        // Checks if the order is valid
    Airlift& operator =(const Airlift& airlift);                            // Assignment operator overloading
    friend ostream& operator<<(ostream& os, Airlift &airlift);              // Stream Insertion Operator
};





// ==================== Negotiate Class ====================
class Negotiate : public Order {
private:
    string orderName;                                                           // Type of the order, in this case Deploy
public:
    Negotiate();                                                                // Default Constructor
    Negotiate(const Negotiate& negotiate);                                      // Copy Constructor
    ~Negotiate();                                                               // Destructor
    bool execute();                                                             // First validates the order, and if valid executes its action
    bool validate();                                                            // Checks if the order is valid
    Negotiate& operator =(const Negotiate& negotiate);                          // Assignment operator overloading
    friend ostream& operator<<(ostream& os, Negotiate &negotiate);              // Stream Insertion Operator
};



