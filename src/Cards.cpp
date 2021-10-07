#include "Player.h"
#include "Cards.h"
#include <algorithm>

// The one true deck, declared as extern in Cards.h
Deck main_deck = Deck();

// CARD
// Card Default constructor
Card::Card()
{

}

// Card constructor which allows it to be instantiated with a type
Card::Card(enum Card_Type type):
    type(type)
{

}

// Card copy constructor
Card::Card(const Card& other):
    type(other.type),
    hand(other.hand)
{

}

// Card destructor
Card::~Card()
{

}

// Assignment operator for card
void Card::operator = (const Card& other)
{
    this->type = other.type;
    this->hand = other.hand;
}

// Stream output operator for a card
std::ostream& operator << (std::ostream& out, Card& source)
{
    int type_index = source.type;
    vector<std::string> types{"bomb", "reinforcement", "blockade", "airlift", "diplomacy"};
    out << "Card[TYPE=" << types[type_index] << "]";
    return out;
}

// Adds an order to the player's order's list based on the type of card
void Card::play()
{
    // Removes the card from the hand
    this->getHand()->removeCard(*this);
    // This card is now not attached to any hand, so set its hand property to NULL
    this->hand = NULL;
    // Add the card back to the deck
    main_deck.addCard(*this);
    // TODO: Issue the order for this card based on its type
    this->getHand()->getPlayer()->addPlayerOrder(new Order());
}

// Returns the type of the card
Card::Card_Type Card::getType() const
{
    return this->type;
}

// Returns the hand which the card is part of
Hand* Card::getHand() const
{
    return this->hand;
}

// Returns the Player who's hand has this card
Player* Hand::getPlayer() const
{
    return this->player;
}

// Sets the hand to a provided hand for a card
// This is called when adding a card to a hand. We need a public setter because we can't access the
// Hand object in Card from inside of Hand directly
void Card::setHand(Hand& hand)
{
    this->hand = &hand;
}

// DECK
// Deck default constructor
Deck::Deck()
{

}

// Deck copy constructor
Deck::Deck(const Deck& other)
{
    deepCopy(other);
}

// Deck destructor which deletes all cards
Deck::~Deck()
{
    for(const Card* card: main_deck.all_cards) {
        delete card;
    }
}

// Assignment operator for the deck
void Deck::operator = (const Deck& other)
{
    deepCopy(other);
}

// Strema output operator for the deck
std::ostream& operator << (std::ostream& out, Deck& source)
{
    out << "Deck[";
    for(Card* card : source.getCards()) {
        out << *card;
    }
    out << "]";
    return out;
}

// Adds a card to the deck given its reference
// Adds the card to main_deck.all_cards as well if it's not already present
// This allows us to later call a single destructor for all the cards
void Deck::addCard(Card& card)
{
    main_deck.getCards().push_back(&card);
    // Add all cards to the all_cards if they're not there already, to then free its memory
    if(find(main_deck.all_cards.begin(), main_deck.all_cards.end(), &card) == main_deck.all_cards.end()) {
        main_deck.all_cards.push_back(&card);
    }
}

// Removes and returns a card at random from the deck
// Returns NULL if trying to draw a card when the deck is empty
Card* Deck::draw()
{
    vector<Card*>& cards = main_deck.getCards();
    int deck_size = cards.size();
    if(deck_size == 0) return NULL;
    int card_index = rand() % deck_size;
    // Save the card so that you can return it
    Card* drawn_card = cards[card_index];
    // Remove the card from the deck
    cards.erase(cards.begin() + card_index);
    return drawn_card;
}

// Returns a reference to the deck's cards
vector<Card*>& Deck::getCards()
{
    return this->cards;
}

// Replaces deck with a deepy copy of the deck provided
void Deck::deepCopy(const Deck& other) {
    // We'll be replacing the deck so delete all the references to the cards
    for(const Card* card : main_deck.all_cards) {
        delete card;
    }
    for(const Card* card : other.all_cards) {
        Card* copied_card = new Card(*card);
        // We copy all instances of this card to all_cards for future cleanup
        this->all_cards.push_back(copied_card);
        // We copy all the cards actually currently in the deck (as opposed to all_cards which includes cards in hands) into the current deck
        if (find(other.cards.begin(), other.cards.end(), &card) != other.cards.end()) {
            this->cards.push_back(copied_card);
        }
    }
}

// Hand
// Default constructor (UNUSED)
Hand::Hand()
{

}

// Constructor which associates a Player with a Hand
Hand::Hand(Player& player):
    player(&player)
{

}

// Copy constructor
Hand::Hand(const Hand& other):
    cards(other.cards)
{

}

// Unused hand destructor because the cards in the hands are already all referenced in deck.all_cards
Hand::~Hand()
{

}

// Assignment operator for a hand
void Hand::operator = (const Hand& other)
{
    this->cards = other.cards;
}

// Stream output operator for a hand
std::ostream& operator << (std::ostream& out, Hand& source)
{
    out << "Hand[";
    for(Card* card : source.getCards()) {
        out << *card;
    }
    out << "]";
    return out;
}

// Adds a card to the hand and sets the Hand of the card to this
void Hand::addCard(Card& card)
{
    this->getCards().push_back(&card);
    card.setHand(*this);
}

// Removes a card in the hand given the card's index in the hand vector
void Hand::removeCard(int card_index)
{
    vector<Card*>& hand = this->getCards();
    if (card_index >= hand.size()) {
        return;
    }
    hand.erase(hand.begin() + card_index);
}

// Removes a card in the hand given a card's reference
void Hand::removeCard(Card& card)
{
    vector<Card*>& hand = this->getCards();
    auto card_to_erase = std::find(hand.begin(), hand.end(), &card);
    // If the card to be removed was not found in the Hand, we've got a problem
    assert(card_to_erase != hand.end());
    if (card_to_erase == hand.end()) {
        return;
    }
    hand.erase(card_to_erase);
}

// Gets a card from the hand given the card's index
Card* Hand::getCard(int card_index)
{
    vector<Card*>& hand = this->getCards();
    if (card_index < 0 || card_index >= hand.size()) {
        return nullptr;
    }
    return hand[card_index];
}

// Returns a reference to the cards in the hand
vector<Card*>& Hand::getCards()
{
    return this->cards;
}
