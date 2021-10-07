#include "Player.h"
#include <iostream>
using std::cout;
using std::endl;

// The one true deck, declared as extern in Cards.h
Deck main_deck;

int main()
{
    // Filling the deck with cards of all types
    main_deck.addCard(*new Card(Card::Card_Type::airlift));
    main_deck.addCard(*new Card(Card::Card_Type::blockade));
    main_deck.addCard(*new Card(Card::Card_Type::bomb));
    main_deck.addCard(*new Card(Card::Card_Type::diplomacy));
    main_deck.addCard(*new Card(Card::Card_Type::reinforcement));
    // Declaring a test hand to demonstrate drawing/playing cards
    Hand hand(*new Player());
    cout << "[DECK] Before drawing any cards: " << main_deck << endl;
    cout << "[HAND] Before adding any cards: " << hand << endl;
    // Draw all the cards in the deck
    while (main_deck.getCards().size() > 0) {
        hand.addCard(*main_deck.draw());
        cout << "[HAND] Right after adding a card: " << hand << endl;
    }
    cout << "[DECK] After drawing all cards: " << main_deck << "\n" << endl;
    // Play all cards in hand
    while(hand.getCards().size() > 0) {
        Card* card = hand.getCard(0);
        cout << "[DECK] Deck BEFORE playing a card and putting it back in the deck: " << main_deck << endl;
        cout << "[HAND] Hand BEFORE playing a card, thus removing it from hand: " << hand << endl;
        cout << "Card played from hand: " << *card << endl;
        card->play();
        cout << "[HAND] Hand AFTER playing a card, thus removing it from hand: " << hand << endl;
        cout << "[DECK] Deck AFTER playing a card and putting it back in the deck: " << main_deck << endl;
    }
    return 0;
}
