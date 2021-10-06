#include "Orders.h"
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <ostream>
using std::vector;

class Hand;
//TODO: Use references instead of pointers when passing in params (Ask ohmni)
class Card
{
    public:
        enum Card_Type {
            bomb,
            reinforcement,
            blockade,
            airlift,
            diplomacy,
            ENUM_SIZE
        };
        Card(enum Card_Type);
        // Copy constructor
        Card(const Card* other);
        // Assignment operator
        void operator = (const Card* other);
        // Stream output operator
        friend std::ostream& operator << (std::ostream& out, const Card* source);
        ~Card();
        Order* play();
        Card_Type getType();
        Hand* getHand();
    private:
        Card_Type type;
        Hand* hand;
};

class Deck
{
    public:
        Deck();
        // Copy constructor
        Deck(const Deck* other);
        ~Deck();
        // Assignment operator
        void operator = (const Deck* other);
        // Stream output operator
        friend std::ostream& operator << (std::ostream& out, const Deck* source);
        vector<Card*>* getCards();
        void addCard(Card* card);
        Card* drawCard();
    private:
        vector<Card*> cards;
};

class Hand
{
    public:
        Hand();
        // Copy constructor
        Hand(const Hand* other);
        // Assignment operator
        void operator = (const Hand* other);
        // Stream output operator
        friend std::ostream& operator << (std::ostream& out, const Hand* source);
        ~Hand();
        void addCard(Card*);
        void removeCard(int);
        void removeCard(Card*);
        Card* getCard(int);
        vector<Card*>* getCards();
    private:
        vector<Card*> cards;
};

extern Deck main_deck;
