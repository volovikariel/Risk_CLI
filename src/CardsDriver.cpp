#include "Player.h"
#include <iostream>
using std::cout;
using std::endl;

int main()
{
    // Filling the deck with cards of all types
    main_deck.addCard(*new Card(Card::Card_Type::airlift));
    main_deck.addCard(*new Card(Card::Card_Type::blockade));
    main_deck.addCard(*new Card(Card::Card_Type::bomb));
    main_deck.addCard(*new Card(Card::Card_Type::diplomacy));
    main_deck.addCard(*new Card(Card::Card_Type::reinforcement));
    // Declaring a test hand to demonstrate drawing/playing cards
    Player p1;
    Player p2;
    Hand hand1(p1);
    Hand hand2(p2);
    OrdersList orders1;
    OrdersList orders2;
    p1.setPlayerCards(&hand1);
    p2.setPlayerCards(&hand2);
    p1.setPlayerOrders(&orders1);
    p2.setPlayerOrders(&orders2);
    cout << "[DECK] Before drawing any cards: " << main_deck << endl;
    cout << "[HAND1] Before adding any cards: " << hand1 << endl;
    cout << "[HAND2] Before adding any cards: " << hand2 << endl;
    cout << endl;

    // Draw all the cards in the deck
    while (main_deck.getCards().size() > 0) {
        // Draw cards 3 cards into hand1 and the rest into hand2
        if (hand1.getCards().size() < 3) {
            hand1.addCard(*main_deck.draw());
        } else {
            hand2.addCard(*main_deck.draw());
        }
        cout << "[HAND1] Right after adding a card: " << hand1 << endl;
        cout << "[HAND2] Right after adding a card: " << hand2 << endl;
    }
    cout << "[DECK] After drawing all cards: " << main_deck << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    // Play all cards in hand1
    while(hand1.getCards().size() > 0) {
        Card* card = hand1.getCard(0);
        cout << "[DECK] Deck BEFORE playing a card and putting it back in the deck: " << main_deck << endl;
        cout << "[HAND1] Hand BEFORE playing a card, thus removing it from hand: " << hand1 << endl;
        cout << "[ORDERLIST1] OrderList BEFORE playing a card, thus adding it to order list: " << endl << (*p1.getPlayerOrders());
        cout << "Card played from hand: " << *card << endl;
        card->play();
        cout << "[HAND1] Hand AFTER playing a card, thus removing it from hand: " << hand1 << endl;
        cout << "[DECK] Deck AFTER playing a card and putting it back in the deck: " << main_deck << endl;
        cout << "[ORDERLIST1] OrderList AFTER playing a card, and adding it to order list: " << endl << (*p1.getPlayerOrders()) << endl;
    }
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    // Play all cards in hand2
    while(hand2.getCards().size() > 0) {
        Card* card = hand2.getCard(0);
        cout << "[DECK] Deck BEFORE playing a card and putting it back in the deck: " << main_deck << endl;
        cout << "[HAND2] Hand BEFORE playing a card, thus removing it from hand: " << hand2 << endl;
        cout << "[ORDERLIST2] OrderList BEFORE playing a card, thus adding it to order list: " << endl << (*p2.getPlayerOrders());
        cout << "Card played from hand: " << *card << endl;
        card->play();
        cout << "[HAND2] Hand AFTER playing a card, thus removing it from hand: " << hand2 << endl;
        cout << "[DECK] Deck AFTER playing a card and putting it back in the deck: " << main_deck << endl;
        cout << "[ORDERLIST2] OrderList AFTER playing a card, and adding it to order list: " << endl << (*p2.getPlayerOrders()) << endl;
    }
    return 0;
}
