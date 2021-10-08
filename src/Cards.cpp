#include "Player.h"
#include "Cards.h"
#include <algorithm>

// The one true deck, declared as extern in Cards.h
Deck main_deck = Deck();

// CARD
// Card Default constructor
Card::Card():
    type(Card_Type::bomb),
    hand(nullptr)
{

}

// Card constructor which allows it to be instantiated with a type
Card::Card(Card_Type type):
    type(type),
    hand(nullptr)
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
    type = other.type;
    hand = other.hand;
}

// Stream insertion operator for a card
std::ostream& operator << (std::ostream& out, const Card& source)
{
    static const vector<std::string> types{ "bomb", "reinforcement", "blockade", "airlift", "diplomacy" };

    int type_index = source.type;
    out << "Card[TYPE=" << types[type_index] << "]";
    return out;
}

// Adds an order to the player's order's list based on the type of card
void Card::play()
{
    // TODO: Issue the order for this card based on its type
    hand->getPlayer()->addPlayerOrder(new Order());

    // Removes the card from the hand
    hand->removeCard(*this);
    // Add the card back to the deck
    main_deck.addCard(*this);
}

// Returns the type of the card
Card::Card_Type Card::getType() const
{
    return type;
}

// Returns the hand which the card is part of
Hand* Card::getHand() const
{
    return hand;
}

// Returns the Player who's hand has this card
Player* Hand::getPlayer() const
{
    return player;
}

// Sets the hand to a provided hand for a card
// This is called when adding a card to a hand. We need a public setter because we can't access the
// Hand object in Card from inside of Hand directly
void Card::setHand(Hand* hand)
{
    this->hand = hand;
}

// DECK
// Deck default constructor
Deck::Deck():
    cards(),
    all_cards()
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
    for (const Card* card : all_cards) {
        delete card;
    }
}

// Assignment operator for the deck
void Deck::operator = (const Deck& other)
{
    deepCopy(other);
}

// Strema insertion operator for the deck
std::ostream& operator << (std::ostream& out, const Deck& source)
{
    out << "Deck[";
    for (const Card* card : source.cards) {
        out << *card;
    }
    out << "]";
    return out;
}

// Adds a card to the deck given its reference
// Adds the card to all_cards as well if it's not already present
// This allows us to later call a single destructor for all the cards
void Deck::addCard(Card& card)
{
    cards.push_back(&card);
    // Add all cards to the all_cards if they're not there already, to destroy them all in Deck::~Deck
    if(find(all_cards.begin(), all_cards.end(), &card) == all_cards.end()) {
        all_cards.push_back(&card);
    }
}

// Removes and returns a card at random from the deck
// Returns NULL if trying to draw a card when the deck is empty
Card* Deck::draw()
{
    size_t deck_size = cards.size();
    if (deck_size == 0) return nullptr;
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
    return cards;
}

// Const version
const vector<Card*>& Deck::getCards() const
{
    return cards;
}

// Replaces deck with a deepy copy of the deck provided
void Deck::deepCopy(const Deck& other) {
    // We'll be replacing the deck so delete all the references to the cards
    for(const Card* card : all_cards) {
        delete card;
    }

    all_cards.clear();
    cards.clear();

    for(const Card* card : other.all_cards) {
        Card* copied_card = new Card(*card);
        // We copy all instances of this card to all_cards for future cleanup
        all_cards.push_back(copied_card);
        // We copy all the cards actually currently in the deck (as opposed to all_cards which includes cards in hands) into the current deck
        if (find(other.cards.begin(), other.cards.end(), card) != other.cards.end()) {
            cards.push_back(copied_card);
        }
    }
}

// Hand
// Default constructor (UNUSED)
Hand::Hand():
    cards(),
    player(nullptr)
{

}

// Constructor which associates a Player with a Hand
Hand::Hand(Player& player):
    cards(),
    player(&player)
{

}

// Copy constructor
Hand::Hand(const Hand& other):
    cards(other.cards),
    player(other.player)
{

}

// Unused hand destructor because the cards in the hands are already all referenced in deck.all_cards
Hand::~Hand()
{

}

// Assignment operator for a hand
void Hand::operator = (const Hand& other)
{
    cards = other.cards;
    player = other.player;
}

// Stream insertion operator for a hand
std::ostream& operator << (std::ostream& out, const Hand& source)
{
    out << "Hand[";
    for(const Card* card : source.cards) {
        out << *card;
    }
    out << "]";
    return out;
}

// Adds a card to the hand and sets the Hand of the card to this
void Hand::addCard(Card& card)
{
    cards.push_back(&card);
    card.setHand(this);
}

// Removes a card in the hand given the card's index in the hand vector
void Hand::removeCard(int card_index)
{
    if (card_index < 0 || card_index >= cards.size()) {
        return;
    }
    // This card is now not attached to any hand, so set its hand property to nullptr
    cards[card_index]->setHand(nullptr);
    cards.erase(cards.begin() + card_index);
}

// Removes a card in the hand given a card's reference
void Hand::removeCard(Card& card)
{
    auto card_to_erase = std::find(cards.begin(), cards.end(), &card);
    if (card_to_erase == cards.end()) {
        return;
    }
    // This card is now not attached to any hand, so set its hand property to nullptr
    (*card_to_erase)->setHand(nullptr);
    cards.erase(card_to_erase);
}

// Gets a card from the hand given the card's index
Card* Hand::getCard(int card_index)
{
    if (card_index < 0 || card_index >= cards.size()) {
        return nullptr;
    }
    return cards[card_index];
}

// Returns a reference to the cards in the hand
vector<Card*>& Hand::getCards()
{
    return cards;
}

// Const version
const vector<Card*>& Hand::getCards() const
{
    return cards;
}
