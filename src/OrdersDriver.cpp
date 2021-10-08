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

    //Add the orders to the list
    list.addOrder(deploy);
    list.addOrder(advance);
    list.addOrder(bomb);
    list.addOrder(blockade);
    list.addOrder(airlift);
    list.addOrder(negotiate);

    //Print the elements of the list
    cout << list << endl;
}
