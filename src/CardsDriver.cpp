#include "Player.h"

#include <iostream>

using std::cout;
using std::endl;

int main()
{
    // Filling the deck with cards of all types
    mainDeck.addCard(*new Card(Card::Type::Airlift));
    mainDeck.addCard(*new Card(Card::Type::Blockade));
    mainDeck.addCard(*new Card(Card::Type::Bomb));
    mainDeck.addCard(*new Card(Card::Type::Diplomacy));
    mainDeck.addCard(*new Card(Card::Type::Reinforcement));

    // Declaring a test hand to demonstrate drawing/playing cards
    Player p1;
    Player p2;
    Hand& hand1 = *p1.getPlayerCards();
    Hand& hand2 = *p2.getPlayerCards();
    OrdersList& orders1 = *p1.getPlayerOrders();
    OrdersList& orders2 = *p2.getPlayerOrders();

    cout << "[DECK] Before drawing any cards: " << mainDeck << endl;
    cout << "[HAND1] Before adding any cards: " << hand1 << endl;
    cout << "[HAND2] Before adding any cards: " << hand2 << endl;
    cout << endl;

    // Draw all the cards in the deck
    while (mainDeck.getCards().size() > 0)
    {
        // Draw cards 3 cards into hand1 and the rest into hand2
        if (hand1.getCards().size() < 3)
        {
            hand1.addCard(*mainDeck.draw());
        }
        else
        {
            hand2.addCard(*mainDeck.draw());
        }

        cout << "[HAND1] Right after adding a card: " << hand1 << endl;
        cout << "[HAND2] Right after adding a card: " << hand2 << endl;
    }

    cout << "[DECK] After drawing all cards: " << mainDeck << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    // Play all cards in hand1
    while(hand1.getCards().size() > 0)
    {
        Card* card = hand1.getCard(0);

        cout << "[DECK] Deck BEFORE playing a card and putting it back in the deck: " << mainDeck << endl;
        cout << "[HAND1] Hand BEFORE playing a card, thus removing it from hand: " << hand1 << endl;
        cout << "[ORDERLIST1] OrderList BEFORE playing a card, thus adding it to order list: " << endl << orders1;
        cout << "Card played from hand: " << *card << endl;

        card->play();

        cout << "[HAND1] Hand AFTER playing a card, thus removing it from hand: " << hand1 << endl;
        cout << "[DECK] Deck AFTER playing a card and putting it back in the deck: " << mainDeck << endl;
        cout << "[ORDERLIST1] OrderList AFTER playing a card, and adding it to order list: " << endl << orders1 << endl;
    }

    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    // Play all cards in hand2
    while(hand2.getCards().size() > 0)
    {
        Card* card = hand2.getCard(0);

        cout << "[DECK] Deck BEFORE playing a card and putting it back in the deck: " << mainDeck << endl;
        cout << "[HAND2] Hand BEFORE playing a card, thus removing it from hand: " << hand2 << endl;
        cout << "[ORDERLIST2] OrderList BEFORE playing a card, thus adding it to order list: " << endl << orders2;
        cout << "Card played from hand: " << *card << endl;

        card->play();

        cout << "[HAND2] Hand AFTER playing a card, thus removing it from hand: " << hand2 << endl;
        cout << "[DECK] Deck AFTER playing a card and putting it back in the deck: " << mainDeck << endl;
        cout << "[ORDERLIST2] OrderList AFTER playing a card, and adding it to order list: " << endl << orders2 << endl;
    }

    return 0;
}
