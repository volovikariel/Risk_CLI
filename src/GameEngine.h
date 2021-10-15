#include <ostream>
#include <string>

using namespace std;

class GameEngine
{
public :

    enum class State
    {
        Start,
        MapLoaded,
        MapValidated,
        PlayersAdded,
        AssignReinforcements,
        IssueOrders,
        ExecuteOrders,
        Win
    };

    // Stream output operator of State
    friend std::ostream& operator << (std::ostream& out, const State source);

    // Default constructor
    GameEngine();
    // Copy constructor
    GameEngine(const GameEngine& other);
    // Destructor
    ~GameEngine();

    // To start the game
    void gameEngineStart();

    // Returns the current state of the engine
    State getState() const;

    // Assignment operator
    GameEngine& operator = (const GameEngine& other);
    // Stream insertion operator
    friend ostream& operator << (ostream& out, const GameEngine& source);

private:

    // Will help set the state when transitioning
    void setState(State state);

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
    State state;
};
