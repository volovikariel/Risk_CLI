#pragma once
#include "LoggingObserver.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ==================== Order Class ====================
class Order : public Subject, public ILoggable
{
public:

    // Enum of order types
    enum class Type
    {
        Order,
        Deploy,
        Advance,
        Bomb,
        Blockade,
        Airlift,
        Negotiate
    };

    // Stream output operator of Order_Type
    friend std::ostream& operator << (std::ostream& out, const Type source);

    Order();                                                            // Default Constructor
    Order(const Order& other);                                          // Copy Constructor
    ~Order();                                                           // Destructor
    explicit Order(const Type orderType);                               // Parameterized Constructor to define the type of Order
    Type getType() const;                                               // Getter for the order type
    Order& operator = (const Order& other);                             // Assignment operator overloading
    friend ostream& operator << (ostream& out, const Order& source);    // Input stream operator
    virtual bool execute();                                             // First validates the order, and if valid executes its action
    virtual bool validate();                                            // Checks if the order is valid
    virtual ostream& print(ostream& out) const;                         // Prints to an output stream
    bool getExecuted() const;                                           // Getter for executed boolean
    void setExecuted(bool value);                                       // Setter for executed boolean
    // Implementation of stringToLog function inherited from ILoggable
    string stringToLog();

protected:

    void setType(Type orderType);                                       // Setter for the order type
    bool executed{};                                                    // Boolean to check if order has been executed or not

private:

    Type orderType;
};

// ==================== OrderList Class ====================
class OrdersList : public Subject, public ILoggable
{
public:

    OrdersList();                                                           // Default Constructor
    OrdersList(const OrdersList& other);                                    // Copy Constructor
    explicit OrdersList(const vector<Order*>& other);
    ~OrdersList();                                                          // Destructor
    void remove(int index);                                                 // Removes the order from the list at specified index
    void move(int from, int to);                                            // Moves an order from position x (from) to position y (to)
    void addOrder(Order* order);                                            // Adds an order to the list
    OrdersList& operator = (const OrdersList& other);                       // Assignment operator overloading
    friend ostream& operator<<(ostream& out, const OrdersList& source);     // Stream insertion operator
    vector<Order*>& getOrdersList();                                        // Getter for orders list
    const vector<Order*>& getOrdersList() const;                            // Const getter for orders list
    // Implementation of stringToLog function inherited from ILoggable
    string stringToLog();

private:

    vector<Order*> orderList;                                               // List (vector) of orders
    void deepCopy(const vector<Order*>& orderList);                         // Deep copy a list of orders
};




// ==================== Deploy Class ====================
class Deploy : public Order
{
public:

    Deploy();                                                           // Default Constructor
    Deploy(const Deploy& other);                                        // Copy Constructor
    ~Deploy();                                                          // Destructor
    bool execute() override;                                            // First validates the order, and if valid executes its action
    bool validate() override;                                           // Checks if the order is valid
    Deploy& operator = (const Deploy& other);                           // Assignment operator overloading
    friend ostream& operator << (ostream& out, const Deploy& source);   // Stream Insertion Operator
    ostream& print(ostream& out) const override;                        // Prints to an output stream
};

// ==================== Advance Class ====================
class Advance : public Order
{
public:

    Advance();                                                          // Default Constructor
    Advance(const Advance& other);                                      // Copy Constructor
    ~Advance();                                                         // Destructor
    bool execute() override;                                            // First validates the order, and if valid executes its action
    bool validate() override;                                           // Checks if the order is valid
    Advance& operator = (const Advance& advance);                       // Assignment operator overloading
    friend ostream& operator << (ostream& out, const Advance& source);  // Stream Insertion Operator
    ostream& print(ostream& out) const override;                        // Prints to an output stream
};

// ==================== Bomb Class ====================
class Bomb : public Order
{
public:

    Bomb();                                                             // Default Constructor
    Bomb(const Bomb& other);                                            // Copy Constructor
    ~Bomb();                                                            // Destructor
    bool execute() override;                                            // First validates the order, and if valid executes its action
    bool validate() override;                                           // Checks if the order is valid
    Bomb& operator = (const Bomb& other);                               // Assignment operator overloading
    friend ostream& operator << (ostream& out, const Bomb& source);     // Stream Insertion Operator
    ostream& print(ostream& out) const override;                        // Prints to an output stream
};

// ==================== Blockade Class ====================
class Blockade : public Order
{
public:

    Blockade();                                                             // Default Constructor
    Blockade(const Blockade& other);                                        // Copy Constructor
    ~Blockade();                                                            // Destructor
    bool execute() override;                                                // First validates the order, and if valid executes its action
    bool validate() override;                                               // Checks if the order is valid
    Blockade& operator = (const Blockade& other);                           // Assignment operator overloading
    friend ostream& operator << (ostream& out, const Blockade& source);     // Stream Insertion Operator
    ostream& print(ostream& out) const override;                            // Prints to an output stream
};

// ==================== Airlift Class ====================
class Airlift : public Order
{
public:

    Airlift();                                                              // Default Constructor
    Airlift(const Airlift& other);                                          // Copy Constructor
    ~Airlift();                                                             // Destructor
    bool execute() override;                                                // First validates the order, and if valid executes its action
    bool validate() override;                                               // Checks if the order is valid
    Airlift& operator = (const Airlift& other);                             // Assignment operator overloading
    friend ostream& operator << (ostream& out, const Airlift& source);      // Stream Insertion Operator
    ostream& print(ostream& out) const override;                            // Prints to an output stream
};

// ==================== Negotiate Class ====================
class Negotiate : public Order
{
public:

    Negotiate();                                                                // Default Constructor
    Negotiate(const Negotiate& other);                                          // Copy Constructor
    ~Negotiate();                                                               // Destructor
    bool execute() override;                                                    // First validates the order, and if valid executes its action
    bool validate() override;                                                   // Checks if the order is valid
    Negotiate& operator = (const Negotiate& other);                             // Assignment operator overloading
    friend ostream& operator << (ostream& out, const Negotiate& source);        // Stream Insertion Operator
    ostream& print(ostream& out) const override;                                // Prints to an output stream
};
