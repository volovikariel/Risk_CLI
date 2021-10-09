#include "Orders.h"
using namespace std;

int main()
{
    //Create an orders list
    OrdersList list;

    //Create each type of order
    Deploy *deploy = new Deploy();
    Advance *advance = new Advance();
    Bomb *bomb = new Bomb();
    Blockade *blockade = new Blockade();
    Airlift *airlift = new Airlift();
    Negotiate *negotiate = new Negotiate();

    //Print the order details
    cout << *deploy << endl;
    cout << *advance << endl;
    cout << *bomb << endl;
    cout << *blockade << endl;
    cout << *airlift << endl;
    cout << *negotiate << endl;

    //Add the orders to the list
    list.addOrder(deploy);
    list.addOrder(advance);
    list.addOrder(bomb);
    list.addOrder(blockade);
    list.addOrder(airlift);
    list.addOrder(negotiate);

    //Print the elements of the list
    cout << "========== Order list contents ==========" << endl;
    cout << list << endl;

    list.removeOrder(2);

    //Print the elements of the list
    cout << "========== Order list after removing index 2 ==========" << endl;
    cout << list << endl;

    list.move(1, 3);

    //Print the elements of the list
    cout << "========== Order list after moving index 1 to index 3 ==========" << endl;
    cout << list << endl;


    Order *order = new Order();

    list.addOrder(order);

    cout << "Adding a new random order" << endl << endl;

    //Print the elements of the list
    cout << "========== Order list after adding a new order ==========" << endl;
    cout << list << endl;


    OrdersList list2 = list;

    //Print the elements of the assign-copied list
    cout << "========== Order list after assignment operator ==========" << endl;
    cout << list2 << endl;
}
