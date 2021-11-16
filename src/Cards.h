#pragma once
#include <assert.h>
#include <ostream>
#include <stdlib.h>
#include <vector>

using std::vector;

// Forward declarations
class Hand;
class Player;
class Order;

// Card class that represents the cards which, depending on their type, allow the player to issue special orders
class Card
{
public:

    // The different card types which issue various orders
    enum class Type
    {
        Bomb,
        Reinforcement,
        Blockade,
        Airlift,
        Diplomacy
    };

    // Default constructor
    Card();
    // Card constructor which allows it to be instantiated with a type
    Card(Type type);
    // Copy constructor
    Card(const Card& other);
    // Card destructor
    ~Card();
    // Assignment operator
    void operator = (const Card& other);
    // Stream insertion operator
    friend std::ostream& operator << (std::ostream& out, const Card& source);
    // Adds an order to the player's order's list based on the type of card
    Order* play();
    // Returns the type of the card
    Type getType() const;
    // Returns the hand which the card is part of
    Hand* getHand() const;
    // Sets the hand to a provided hand for a card. Provide nullptr to clear.
    void setHand(Hand* hand);

private:

    // The type of the card which influences what happens when it's played
    Type type;
    // What hand the card is currently a part of, NULL if it's part of the deck and not part of a hand
    Hand* hand;
};

// Deck class which is setup once and from then on, the Player's can draw a finite collection of cards from it into their hand
class Deck
{
public:

    // Default constructor
    Deck();
    // Copy constructor
    Deck(const Deck& other);
    // Destructor
    ~Deck();
    // Assignment operator
    void operator = (const Deck& other);
    // Stream insertion operator
    friend std::ostream& operator << (std::ostream& out, const Deck& source);
    // Adds a random amount of cards (with even distribution of card types)
    void addRandomCards(int numCards);
    // Adds a card to the deck given its reference (adds the card to mainDeck.allCards as well if it's not already present)
    void addCard(Card& card);
    // Removes and returns a card at random from the deck (returns NULL if trying to draw a card when the deck is empty)
    Card* draw();
    // Returns a reference to the deck's cards
    vector<Card*>& getCards();
    // Const version
    const vector<Card*>& getCards() const;

private:

    // Replaces deck with a deepy copy of the deck provided
    void deepCopy(const Deck& other);

    // The cards currently present in the deck
    vector<Card*> cards;
    // All the cards that were ever instantiated, be they in the deck or in a player's hand
    // It's here to facilitat ethe job of cleaning up
    vector<Card*> allCards;
};

// Each player has a hand which may or may not contain some cards which allow them to issue special orders
class Hand
{
public:

    // Constructor which associates a Player with a Hand
    Hand(Player& player);
    // Copy constructor
    Hand(const Hand& other);
    // Unused hand destructor because the cards in the hands are already all referenced in deck.allCards
    ~Hand();
    // Assignment operator
    void operator = (const Hand& other);
    // Stream insertion operator
    friend std::ostream& operator << (std::ostream& out, const Hand& source);
    // Adds a card to the hand and sets the Hand of the card to this
    void addCard(Card& card);
    // Removes a card in the hand given the card's index in the hand vector
    void removeCard(int index);
    // Removes a card in the hand given a card's reference
    void removeCard(Card& card);
    // Gets a card from the hand given the card's index
    Card* getCard(int index);
    // Returns the Player who's hand has this card
    Player* getPlayer() const;
    // Returns a reference to the cards in the hand
    vector<Card*>& getCards();
    // Const version
    const vector<Card*>& getCards() const;

private:

    // Default constructor isn't allowed. A hand must always be attached to a Player
    Hand();
    // The cards present in the hand
    vector<Card*> cards;
    // The player which has this hand
    Player* player;
};

// Seeing as there's a single deck, we define the deck as extern and use it everywhere else.
// This way, we don't have to mess around with copying over decks or using getters.
extern Deck mainDeck;
