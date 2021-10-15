#include <ostream>
#include <string>
#include <vector>

using namespace std;

// Forward declaration
class StateInfo;

// Runs the game logic
class GameEngine
{
public :

    // GameEngine State node
    enum class State
    {
        Start,
        MapLoaded,
        MapValidated,
        PlayersAdded,
        AssignReinforcements,
        IssueOrders,
        ExecuteOrders,
        Win,
        Quit,
        NumStates
    };

    // Stream output operator of State
    friend std::ostream& operator << (std::ostream& out, const State source);

    // GameEngine transitions between States
    enum class Transition
    {
        LoadMap,
        ValidateMap,
        AddPlayer,
        GameStart,
        IssueOrder,
        IssueOrdersEnd,
        ExecOrder,
        ExecOrdersEnd,
        Win,
        Replay,
        Quit,
        NumTransitions
    };

    // Stream output operator of Transition
    friend std::ostream& operator << (std::ostream& out, const Transition source);

    // Default constructor
    GameEngine();
    // Copy constructor
    GameEngine(const GameEngine& other);
    // Destructor
    ~GameEngine();

    // Assignment operator
    GameEngine& operator = (const GameEngine& other);
    // Stream insertion operator
    friend ostream& operator << (ostream& out, const GameEngine& source);

    // Returns the current state of the engine
    State getState() const;

    // Returns the current state of the engine
    const StateInfo& getStateInfo() const;

    // Transitions to the provided state, if valid. Returns the valid status.
    bool transitionState(State state);

    // Performs the provided state transition, if valid. Returns the valid status.
    bool transitionState(Transition transition);

private:

    // Holds the current state
    State state;
};

// Describes a State's graph connectivity
class StateInfo
{
public:

    // Default constructor
    StateInfo();
    // Parametrized constructor
    StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>& stateTransitions);
    // Copy constructor
    StateInfo(const StateInfo& other);
    // Destructor
    ~StateInfo();

    // Assignment operator
    StateInfo& operator = (const StateInfo& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const StateInfo& source);

    const vector<pair<GameEngine::State, GameEngine::Transition>> getStateTransitions() const;
    const vector<GameEngine::State> getStates() const;
    const vector<GameEngine::Transition> getTransitions() const;

    bool canDoState(GameEngine::State state) const;
    bool canDoState(GameEngine::State state, GameEngine::Transition& result) const;
    bool canDoTransition(GameEngine::Transition state) const;
    bool canDoTransition(GameEngine::Transition state, GameEngine::State& result) const;

private:

    // Contains all graph neighbors of this state
    vector<pair<GameEngine::State, GameEngine::Transition>> stateTransitions;

    // Contains all directly connected states
    vector<GameEngine::State> states;

    // Contains all possible transitions
    vector<GameEngine::Transition> transitions;
};

// Describes the game engine's graph connectivity
class StateGraphInfo
{
public:

    // Default constructor
    StateGraphInfo();
    // Copy constructor
    StateGraphInfo(const StateGraphInfo& other);
    // Destructor
    ~StateGraphInfo();

    // Assignment operator
    StateGraphInfo& operator = (const StateGraphInfo& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const StateGraphInfo& source);

    // Returns the connectivity info of a state
    const StateInfo& getStateInfo(GameEngine::State state) const;

private:

    // Contains an entry for each GameEngine::State
    vector<StateInfo*> states;
};

namespace StringUtils
{
    // Converts a string to an integer. Returns success value.
    bool ToInteger(std::string& str, int& result);

    // Converts a string to an integer which must be positive. Returns success value.
    bool ToUnsignedInteger(std::string& str, int& result);

    // Transform the string to lower-case. Result is stored in the same string.
    void ToLowerCase(std::string& str);

    // Returns the GameEngine::Transition corresponding to a user input string.
    // Not case sensitive.
    // Returns success value.
    bool ToGameEngineTransition(std::string& str, GameEngine::Transition& result);
};

// Global StateGraph object for queries about states and transitions
extern StateGraphInfo stateGraphInfo;
