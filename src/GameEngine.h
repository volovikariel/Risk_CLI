#pragma once
#include "LoggingObserver.h"
#include <ostream>
#include <string>
#include <vector>

using namespace std;

// Forward declaration
class Command;
class Map;
class Player;
class StateInfo;

// Runs the game logic
class GameEngine : public Subject, public ILoggable
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
        Tournament,
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
    bool transition(State state);

    // Performs the provided state transition, if valid. Returns the valid status.
    bool transition(Transition transition);

    // Returns the currently loaded map
    Map& getMap();

    // Returns the current players
    vector<Player*>& getPlayers();

    // Returns the current non-eliminated players
    vector<Player*>& getAlivePlayers();

    // Returns the current eliminated players
    vector<Player*>& getEliminatedPlayers();

    // Add a new player to the game.
    // If canRename is true, player name collisions are handled by adding an index.
    // Ex Player, Player2, Player3
    bool addPlayer(Player& player, bool canRename);

    // Returns the neutral player
    Player& getNeutralPlayer();

    // Performs initial setup with given map and players
    bool start(string mapFilepath, vector<Player*>& players);

    // Executes a command. Returns a success status.
    bool executeCommand(Command& command);

    // Creates main game loop for the game
    void mainGameLoop();

    // Executes a single turn of the game
    void executeTurn();

    // Implementation of stringToLog function inherited from ILoggable
    string stringToLog();

    // Incremented every turn
    // Doesn't get reset between games
    int getTurnIndex() const;

private:

    // Call before starting a new game
    void cleanup(bool prepareNewGame);

    // Calculates Reinforcements for players
    void reinforcementPhase();

    // Queues players orders
    void issueOrdersPhase();

    // Executes queued player orders
    void executeOrdersPhase();

    // Verifies if a player need to be eliminated
    void eliminatePlayers();

    // Holds the current state
    State state;

    // Holds the currently loaded map
    Map* map;

    // Holds the players
    vector<Player*> players;

    // Holds non-eliminated players
    vector<Player*> alivePlayers;

    // Holds eliminated players
    vector<Player*> eliminatedPlayers;

    // Neutral player
    Player* neutralPlayer;

    // Incremented every turn
    // Doesn't get reset between games
    int turnIndex;
};

// Describes a State's graph connectivity
class StateInfo
{
public:

    // Default constructor
    StateInfo();
    // Parametrized constructor
    StateInfo(const vector<pair<GameEngine::State, GameEngine::Transition>>& stateTransitions);
    // Copy constructor
    StateInfo(const StateInfo& other);
    // Destructor
    ~StateInfo();

    // Assignment operator
    StateInfo& operator = (const StateInfo& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const StateInfo& source);

    // Returns (State, Transition) pairs of direct graph connections
    const vector<pair<GameEngine::State, GameEngine::Transition>> getStateTransitions() const;
    // Returns directly connected states
    const vector<GameEngine::State> getStates() const;
    // Returns possible transitions
    const vector<GameEngine::Transition> getTransitions() const;

    // Checks if this state can directly transition to the provided state
    bool canDoState(GameEngine::State state) const;
    // Checks if this state can directly transition to the provided state. If yes, stores the associated transition in result.
    bool canDoState(GameEngine::State state, GameEngine::Transition& result) const;
    // Checks if this state can perform the provided transition
    bool canDoTransition(GameEngine::Transition state) const;
    // Checks if this state can perform the provided transition. If yes, stores the State associated to this transition in result.
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

// Helper functions for string processing
namespace StringUtils
{
    // Converts a string to an integer. Returns success value.
    bool ToInteger(std::string& str, int& result);

    // Converts a string to an integer which must be positive. Returns success value.
    bool ToUnsignedInteger(std::string& str, int& result);

    // Transform the string to lower-case. Result is stored in the same string.
    // Returns a reference to the same string, not a new one.
    std::string& ToLowerCase(std::string& str);

    // Adds spaces to a string so that it occupies exactly numSpaces of character space.
    std::string SpacePad(const std::string& str, size_t numSpaces);

    // Extracts the filename from a filepath
    std::string Filename(const std::string& filepath);

    // Returns the GameEngine::Transition corresponding to a user input string.
    // Handles similar words (ex: quit/exit, endissueorders/issueordersend, etc).
    // Not case sensitive.
    // Returns success value.
    bool ToGameEngineTransition(std::string& str, GameEngine::Transition& result);
};

// Global StateGraph object for queries about states and transitions
extern StateGraphInfo stateGraphInfo;
