#include "Orders.h"
#include <stdlib.h>
#include <assert.h>
#include <vector>
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
        Card(const Card&);
        // Assignment operator
        void operator = (const Card&);
        // Stream output operator
        friend std::ostream& operator << (std::ostream&, Card&);
        ~Card();
        Order* play();
        Card_Type getType() const;
        Hand* getHand() const;
    private:
        Card_Type type;
        Hand* hand;
};

class Deck
{
    public:
        Deck();
        // Copy constructor
        Deck(const Deck&);
        ~Deck();
        // Assignment operator
        void operator = (const Deck&);
        // Stream output operator
        friend std::ostream& operator << (std::ostream&, Deck&);
        vector<Card*>& getCards();
        void addCard(Card&);
        Card* drawCard();
    private:
        vector<Card*> cards;
        // TODO: Use this
        vector<Card*> all_cards;
};

class Hand
{
    public:
        Hand();
        // Copy constructor
        Hand(const Hand&);
        // Assignment operator
        void operator = (const Hand&);
        // Stream output operator
        friend std::ostream& operator << (std::ostream&, Hand&);
        ~Hand();
        void addCard(Card&);
        void removeCard(int);
        void removeCard(Card&);
        Card* getCard(int);
        vector<Card*>& getCards();
    private:
        vector<Card*> cards;
};

extern Deck main_deck;
