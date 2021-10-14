#include "Player.h"


int main(int argc, char *argv[])
{
    Player* p = new Player();
    vector<Territory*> territories;
    vector<Territory*> territoriesToDefend;
    vector<Territory*> territoriesToAttack;
    Continent c1(1, "North America", "Red", 100);
    Territory t1(1, "Canada", &c1, 50, 100);
    Territory t2(2, "United States", &c1, 100, 100);
    Continent c2(1, "Asia", "Red", 100);
    Territory t3(1, "Russia", &c2, 1000, 100);
    Territory t4(2, "China", &c2, 100, 50);
    Order* o1 = new Order(Order::Order_Type::bomb);
    Order* o2 = new Order(Order::Order_Type::advance);
    Hand* h = new Hand(*p);
    h->addCard(*new Card(Card::Type::Bomb));
    h->addCard(*new Card(Card::Type::Airlift));


    // 1) Verifying that players have a collection of territories

    cout << "\n===1) Verifying that players have a collection of territories===" << endl;
    territories.push_back(&t1);
    territories.push_back(&t2);
    p->setPlayerTerritories(territories);
    cout << *p;


    // 2) Verifying that players have a collection of cards (i.e a hand of cards)

    cout << "\n===2) Verifying that players have a collection of cards (i.e a hand of cards)===" << endl;
    p->setPlayerCards(h);
    cout << *p;


    // 3) Verifying that players have a collection of territories returned by .toDefend()

    cout << "\n===3) Verifying that players have a collection of territories returned by .toDefend()===" << endl;
    territoriesToDefend = territories;
    p->setTerritoriesToDefend(territoriesToDefend);
    cout << "\nTerritories to defend: " << endl;
    for (Territory* t : p->toDefend())
    {
        cout << *t << endl;
    }


    // 4) Verifying that players have a collection of territories returned by .toAttack()

    cout << "\n===4) Verifying that players have a collection of territories returned by .toAttack()===" << endl;
    territoriesToAttack.push_back(&t3);
    territoriesToAttack.push_back(&t4);
    p->setTerritoriesToAttack(territoriesToAttack);
    cout << "\nTerritories to attack: " << endl;
    for (Territory* t : p->toAttack())
    {
        cout << *t << endl;
    }


    // 5) Verifying that players can use the method .issueOrder() to add an order to their list of orders

    cout << "\n===5) Verifying that players can use the method .issueOrder() to add an order to their list of orders===" << endl;
    p->issueOrder(o1);
    p->issueOrder(o2);
    cout << *p;
}
