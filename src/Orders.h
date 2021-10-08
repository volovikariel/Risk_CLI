#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ==================== Order Class ====================
class Order {
public:
    enum Order_Type {
        order,
        deploy,
        advance,
        bomb,
        blockade,
        airlift,
        negotiate
    };

    // Stream output operator of Order_Type
    friend std::ostream& operator << (std::ostream& out, const Order_Type source);

    Order();                                                            // Default Constructor
    Order(const Order& copy);                                           // Copy Constructor
    ~Order();                                                           // Destructor
    explicit Order(const Order_Type& orderType);                        // Parameterized Constructor to define the type of Order
    Order_Type getType() const;                                         // Getter for the order type
    Order & operator=(const Order& order);                              // Assignment operator overloading
    friend ostream& operator <<(ostream& os, Order& order);             // Input stream operator
    virtual bool execute();                                             // First validates the order, and if valid executes its action
    virtual bool validate();                                            // Checks if the order is valid
    bool getExecuted();                                                 // Getter for executed boolean
    bool setExecuted(const bool& value);                                // Setter for executed boolean
private:
    Order_Type orderType;
protected:
    void setType(const Order_Type& orderType);                          // Setter for the order type
    bool executed;                                                      // Boolean to check if order has been executed or not
};


// ==================== OrderList Class ====================
class OrdersList {
private:
    vector<Order*> orderList;                                           // List (vector) of orders
    void deepCopy(const vector<Order*>& orderList);                     // Deep copy a list of orders

public:
    OrdersList();                                                       // Default Constructor
    OrdersList(const OrdersList& copy);                                 // Copy Constructor
    explicit OrdersList(const vector<Order*>& orderList);
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
public:
    Negotiate();                                                                // Default Constructor
    Negotiate(const Negotiate& negotiate);                                      // Copy Constructor
    ~Negotiate();                                                               // Destructor
    bool execute();                                                             // First validates the order, and if valid executes its action
    bool validate();                                                            // Checks if the order is valid
    Negotiate& operator =(const Negotiate& negotiate);                          // Assignment operator overloading
    friend ostream& operator<<(ostream& os, Negotiate &negotiate);              // Stream Insertion Operator
};
