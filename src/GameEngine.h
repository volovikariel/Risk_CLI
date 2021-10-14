#include <ostream>
#include <string>

using namespace std;

class GameEngine
{
public :

    // Default constructor
    GameEngine();
    // Copy constructor
    GameEngine(const GameEngine& other);
    // Destructor
    ~GameEngine();

    // To start the game
    void gameEngineStart();

    // Will help set the state when transitioning
    void setState(const string &state);

    // Assignment operator
    GameEngine& operator = (const GameEngine& other);
    // Stream insertion operator
    friend ostream& operator << (ostream &out, const GameEngine& source);
    
private:

    // Shows if the map is loaded
    void mapLoaded();
    // Shows if the map loaded is valid
    void mapValidated();
    // Adds player or assigns countries
    void playersAdded();
    // Determines what orders are issued
    void assignReinforcements();
    // Shows if the orders have to end or more to be added
    void issueOrders();
    // Determine if there is a winner or more needs to be done
    void executeOrders();
    // Determines if the game will end or another will be played
    void win();

    // Stores user input
    string userInput;
    // Holds the current state
    string state;
};
