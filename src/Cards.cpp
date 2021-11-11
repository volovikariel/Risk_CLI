#include "Cards.h"
#include "Player.h"

#include <algorithm>

// The one true deck, declared as extern in Cards.h
Deck mainDeck = Deck();

// CARD
// Card Default constructor
Card::Card():
    type(Card::Type::Bomb),
    hand(nullptr)
{

}

// Card constructor which allows it to be instantiated with a type
Card::Card(Card::Type type):
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

    int typeIndex = static_cast<int>(source.type);
    out << "Card[TYPE=" << types[typeIndex] << "]";
    return out;
}

// Adds an order to the player's order's list based on the type of card
void Card::play()
{
    switch (type)
    {
    case Card::Type::Bomb:
        hand->getPlayer()->issueOrder(new Bomb());
        break;
    case Card::Type::Reinforcement:
        // This card doesn't create an order
        break;
    case Card::Type::Blockade:
        hand->getPlayer()->issueOrder(new Blockade());
        break;
    case Card::Type::Airlift:
        hand->getPlayer()->issueOrder(new Airlift());
        break;
    case Card::Type::Diplomacy:
        hand->getPlayer()->issueOrder(new Negotiate());
        break;
    default:
        break;
    }

    // Removes the card from the hand
    hand->removeCard(*this);
    // Add the card back to the deck
    mainDeck.addCard(*this);
}

// Returns the type of the card
Card::Type Card::getType() const
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
    allCards()
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
    for (const Card* card : allCards)
    {
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
    for (const Card* card : source.cards)
    {
        out << *card;
    }
    out << "]";
    return out;
}

// Adds a card to the deck given its reference
// Adds the card to allCards as well if it's not already present
// This allows us to later call a single destructor for all the cards
void Deck::addCard(Card& card)
{
    cards.push_back(&card);
    // Add all cards to the allCards if they're not there already, to destroy them all in Deck::~Deck
    if (find(allCards.begin(), allCards.end(), &card) == allCards.end())
    {
        allCards.push_back(&card);
    }
}

// Removes and returns a card at random from the deck
// Returns NULL if trying to draw a card when the deck is empty
Card* Deck::draw()
{
    size_t deckSize = cards.size();
    if (deckSize == 0)
        return nullptr;

    int cardIndex = rand() % deckSize;
    // Save the card so that you can return it
    Card* drawnCard = cards[cardIndex];
    // Remove the card from the deck
    cards.erase(cards.begin() + cardIndex);

    return drawnCard;
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
void Deck::deepCopy(const Deck& other)
{
    // We'll be replacing the deck so delete all the references to the cards
    for (const Card* card : allCards)
    {
        delete card;
    }

    allCards.clear();
    cards.clear();

    for (const Card* card : other.allCards)
    {
        Card* copiedCard = new Card(*card);
        // We copy all instances of this card to allCards for future cleanup
        allCards.push_back(copiedCard);
        // We copy all the cards actually currently in the deck (as opposed to allCards which includes cards in hands) into the current deck
        if (find(other.cards.begin(), other.cards.end(), card) != other.cards.end())
        {
            cards.push_back(copiedCard);
        }
    }
}

// Hand
// Default constructor
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

// Unused hand destructor because the cards in the hands are already all referenced in deck.allCards
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
    for (const Card* card : source.cards)
    {
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
void Hand::removeCard(int cardIndex)
{
    unsigned int index = cardIndex;
    if (index >= cards.size())
    {
        return;
    }
    // This card is now not attached to any hand, so set its hand property to nullptr
    cards[index]->setHand(nullptr);
    cards.erase(cards.begin() + index);
}

// Removes a card in the hand given a card's reference
void Hand::removeCard(Card& card)
{
    auto cardToErase = std::find(cards.begin(), cards.end(), &card);
    if (cardToErase == cards.end())
    {
        return;
    }
    // This card is now not attached to any hand, so set its hand property to nullptr
    (*cardToErase)->setHand(nullptr);
    cards.erase(cardToErase);
}

// Gets a card from the hand given the card's index
Card* Hand::getCard(int cardIndex)
{
    unsigned int index = cardIndex;
    if (index >= cards.size())
    {
        return nullptr;
    }
    return cards[index];
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
