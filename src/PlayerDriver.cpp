#include "Player.h"


int main(int argc, char *argv[])
{
    Player* p = new Player();
    vector<Territory*> territories;
    Continent* c = new Continent(1, "North America", "Red", 100);
    Territory* t1 = new Territory(1, "Canada", c, 50, 100);
    Territory* t2 = new Territory(2, "United States", c, 100, 100);
    /*Order* order = new Order();
    Deck* deck = new Deck();
    deck->push_card(CardType::SPY);
    Hand* h = new Hand(deck);*/

    //1) Verifying that players have a collection of territories

    cout << "\n===1) Verifying that players have a collection of territories===" << endl;
    territories.push_back(t1);
    territories.push_back(t2);
    p->setPlayerTerritories(territories);
    cout << *p;

    /*
    //2) Verifying that players have a collection of cards (i.e a hand of cards)

    cout << "\n===2) Verifying that players have a collection of cards (i.e a hand of cards)===" << endl;
    p->setPlayerCards(h);
    cout << *p;*/

    //3) Verifying that players have a collection of territories returned by .toDefend()

    cout << "\n===3) Verifying that players have a collection of territories returned by .toDefend()===" << endl;
    vector<Territory*> territoriesToDefend = p->toDefend();
    cout << "\nTerritories to defend: " << endl;
    for (Territory* t : territoriesToDefend) {
        cout << *t << endl;
    }

    //4) Verifying that players have a collection of territories returned by .toAttack()

    cout << "\n===4) Verifying that players have a collection of territories returned by .toAttack()===" << endl;
    vector<Territory*> territoriesToAttack = p->toAttack();
    cout << "\nTerritories to attack: " << endl;
    for (Territory* t : territoriesToAttack) {
        cout << *t << endl;
    }

    /*
    //5) Verifying that players can use the method .issueOrder() to add an order to their list of orders

    cout << "\n===5) Verifying that players can use the method .issueOrder() to add an order to their list of orders===" << endl;
    p->issueOrder(order);
    cout << *p;*/
}