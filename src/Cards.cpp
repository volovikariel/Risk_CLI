#include "Cards.h"

#include <stdlib.h>
#include <vector>
#include <time.h>
#include <assert.h>
#include <iostream>
#include <algorithm>

// CARD
// TODO: This returns an Order* with the card type, etc.
// TODO: Remove card from hand->..somehow?
Card::Card(enum Card_Type type):
    type(type)
{

}

Card::Card(const Card* other):
    type(other->type),
    hand(other->hand)
{

}


Card::~Card()
{

}

void Card::operator = (const Card* other)
{
    this->type = other->type;
    this->hand = other->hand;
}

std::ostream& operator << (std::ostream& out, const Card* source)
{
    out << "[" << source->type << "] " << source->hand;
    return out;
}

Order* Card::play()
{
    this->getHand()->removeCard(this);
    main_deck.addCard(this);
    // TODO: Provide an order type
    //return new Order(this->type);
    return new Order();
}

Card::Card_Type Card::getType()
{
    return this->type;
}

Hand* Card::getHand()
{
    return this->hand;
}

// DECK
Deck::Deck()
{

}

Deck::Deck(const Deck* other):
    cards(other->cards)
{

}

Deck::~Deck()
{
    for(const Card* card: *(main_deck.getCards())) {
        delete card;
    }
}

void Deck::operator = (const Deck* other)
{
    this->cards = other->cards;
}

std::ostream& operator << (std::ostream& out, const Deck* source)
{
    for(Card* card : source->cards) {
        out << card;
    }
    return out;
}

void Deck::addCard(Card* card)
{
    main_deck.getCards()->push_back(card);
}

// Removes and returns a card at random from the deck
Card* Deck::drawCard()
{
    vector<Card*>* cards = main_deck.getCards();
    int deck_size = cards->size();
    if(deck_size == 0) return NULL;
    int card_index = rand() % deck_size;
    // Save the card so that you can return it
    Card* drawn_card = (*cards)[card_index];
    // Remove the card from the deck
    cards->erase(cards->begin() + card_index);
    return drawn_card;
}

vector<Card*>* Deck::getCards()
{
    return &this->cards;
}

// Hand
Hand::Hand()
{

}

Hand::Hand(const Hand* other):
    cards(other->cards)
{

}

Hand::~Hand()
{

}

void Hand::operator = (const Hand* other)
{
    this->cards = other->cards;
}

std::ostream& operator << (std::ostream& out, const Hand* source)
{
    for(Card* card : source->cards) {
        out << card;
    }
    return out;
}

void Hand::addCard(Card* card)
{
    this->getCards()->push_back(card);
}

Card* Hand::getCard(int card_index)
{
    vector<Card*>* hand = this->getCards();
    if (card_index < 0 || card_index >= hand->size()) {
        return NULL;
    }
    return (*hand)[card_index];
}

void Hand::removeCard(int card_index)
{
    vector<Card*>* hand = this->getCards();
    if (card_index >= hand->size()) {
        return;
    }
    hand->erase(hand->begin() + card_index);
}

void Hand::removeCard(Card* card)
{
    vector<Card*>* hand = this->getCards();
    auto card_to_erase = std::find(hand->begin(), hand->end(), card);
    // If the card to be removed was not found in the Hand, we've got a problem
    assert(card_to_erase != hand->end());
    if (card_to_erase == hand->end()) {
        return;
    }
    hand->erase(card_to_erase);
}


vector<Card*>* Hand::getCards()
{
    return &this->cards;
}
