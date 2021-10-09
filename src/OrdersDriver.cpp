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
    cout << *deploy << endl;        //bool executed = true (hard coded for demonstration)
    cout << *advance << endl;
    cout << *bomb << endl;          //bool executed = true (hard coded for demonstration)
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

    //Remove order at index 2
    list.remove(2);

    //Print the elements of the list
    cout << "========== Order list after removing index 2 ==========" << endl;
    cout << list << endl;

    //Move order from index 1 to index 3
    list.move(1, 3);

    //Print the elements of the list
    cout << "========== Order list after moving index 1 to index 3 ==========" << endl;
    cout << list << endl;

    //Creating a second orders list and doing a deep copy of original list
    OrdersList list2 = list;

    //Print the elements of the assign-copied list
    cout << "========== New list with deep copy (list2) ==========" << endl;
    cout << list2 << endl;

    //Remove and add a new order to list2
    list2.remove(0);
    list2.remove(2);
    list2.addOrder(new Bomb());

    //Print the elements of list2 after modification
    cout << "========== Elements of list2 after modifications ==========" << endl;
    cout << list2 << endl;

    //Print the elements of list1 to show that it is unchanged and is a true deep copy
    cout << "========== Elements of list1 to show deep copy didn't change list1 ==========" << endl;
    cout << list << endl;

    deploy->validate();
    deploy->execute();

    airlift->execute();
    airlift->validate();

}
