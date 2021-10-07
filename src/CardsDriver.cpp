#include "Cards.h"
#include <time.h>
#include <iostream>

Deck main_deck;

int main()
{
    const int DECK_SIZE = 5;
    main_deck.addCard(*new Card(Card::Card_Type::airlift));
    main_deck.addCard(*new Card(Card::Card_Type::blockade));
    main_deck.addCard(*new Card(Card::Card_Type::bomb));
    main_deck.addCard(*new Card(Card::Card_Type::diplomacy));
    main_deck.addCard(*new Card(Card::Card_Type::reinforcement));
    // Ariel: A way to add a fixed amount of cards to a deck
    //srand((unsigned) time(NULL));
    //for(int i = 0; i < DECK_SIZE; ++i) {
    //    int enum_index = rand() % Card::Card_Type::ENUM_SIZE;
    //    deck.add(new Card(static_cast<Card::Card_Type>(enum_index)));
    //}
    Hand hand;
    while (main_deck.getCards().size() > 0) {
        hand.addCard(*main_deck.drawCard());
    }
    while(hand.getCards().size() > 0) {
        Card* card = hand.getCard(0);
        card->play();
        //std::cout << hand << std::endl;
        hand.removeCard(0);
    }
    return 0;
}
