#pragma once
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <ostream>
using std::vector;

// mini-declaring Hand so as to be able to use it in Card before Hand was fully declared
class Hand;
class Player;
class Order;

// Card class that represents the cards which, depending on their type, allow the player to issue special orders
class Card
{
    public:
        // The different card types which issue various orders
        enum Card_Type {
            bomb,
            reinforcement,
            blockade,
            airlift,
            diplomacy
        };
        // Default constructor
        Card();
        // Card constructor which allows it to be instantiated with a type
        Card(enum Card_Type);
        // Copy constructor
        Card(const Card&);
        // Card destructor
        ~Card();
        // Assignment operator
        void operator = (const Card&);
        // Stream insertion operator
        friend std::ostream& operator << (std::ostream&, Card&);
        // Adds an order to the player's order's list based on the type of card
        void play();
        // Returns the type of the card
        Card_Type getType() const;
        // Returns the hand which the card is part of
        Hand* getHand() const;
        // Sets the hand to a provided hand for a card
        void setHand(Hand&);
    private:
        // The type of the card which influences what happens when it's played
        Card_Type type;
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
        Deck(const Deck&);
        // Destructor
        ~Deck();
        // Assignment operator
        void operator = (const Deck&);
        // Stream insertion operator
        friend std::ostream& operator << (std::ostream&, Deck&);
        // Adds a card to the deck given its reference (adds the card to main_deck.all_cards as well if it's not already present)
        void addCard(Card&);
        // Removes and returns a card at random from the deck (returns NULL if trying to draw a card when the deck is empty)
        Card* draw();
        // Returns a reference to the deck's cards
        vector<Card*>& getCards();
        // Replaces deck with a deepy copy of the deck provided
        void deepCopy(const Deck&);
    private:
        // The cards currently present in the deck
        vector<Card*> cards;
        // All the cards that were ever instantiated, be they in the deck or in a player's hand
        // It's here to facilitat ethe job of cleaning up
        vector<Card*> all_cards;
};

// Each player has a hand which may or may not contain some cards which allow them to issue special orders
class Hand
{
    public:
        // Constructor which associates a Player with a Hand
        Hand(Player&);
        // Copy constructor
        Hand(const Hand&);
        // Unused hand destructor because the cards in the hands are already all referenced in deck.all_cards
        ~Hand();
        // Assignment operator
        void operator = (const Hand&);
        // Stream insertion operator
        friend std::ostream& operator << (std::ostream&, Hand&);
        // Adds a card to the hand and sets the Hand of the card to this
        void addCard(Card&);
        // Removes a card in the hand given the card's index in the hand vector
        void removeCard(int);
        // Removes a card in the hand given a card's reference
        void removeCard(Card&);
        // Gets a card from the hand given the card's index
        Card* getCard(int);
        // Returns the Player who's hand has this card
        Player* getPlayer() const;
        // Returns a reference to the cards in the hand
        vector<Card*>& getCards();
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
extern Deck main_deck;
