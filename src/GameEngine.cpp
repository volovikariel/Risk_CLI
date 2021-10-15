#include "GameEngine.h"

#include <iostream>

// Stream output operator of State
std::ostream& operator << (std::ostream& out, const GameEngine::State source)
{
    static const char* names[8] =
    {
        "Start",
        "MapLoaded",
        "MapValidated",
        "PlayersAdded",
        "AssignReinforcements",
        "IssueOrders",
        "ExecuteOrders",
        "Win"
    };

    out << names[static_cast<size_t>(source)];
    return out;
}

// Constructor called when object is created. will set the state to start when game begins
GameEngine::GameEngine():
    state(State::Start)
{

}

// Destructor
GameEngine::~GameEngine()
{

}

// Returns the current state of the engine
GameEngine::State GameEngine::getState() const
{
    return state;
}

// Will help set the next state
void GameEngine::setState(State state)
{
    this->state = state;
}

// Stream insertion operator
ostream &operator << (ostream& out, const GameEngine& source)
{
    out << "state: " << source.state;
    return out;
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& other):
    state(other.state)
{

}

// Assignment operator
GameEngine& GameEngine::operator = (const GameEngine& other)
{
    this->state = other.state;
    return *this;
}

// Starts the game
void GameEngine::gameEngineStart()
{
    cout << "You are now in the " << state << " state of the Game menu." << endl;

    cout << "Please enter the following command to start the game : loadmap --> " << endl;
    getline(cin, userInput);
    cout << "" << endl;

    if (userInput == "loadmap")
    {
        cout << "Transitioning to the next state --> Map Loaded..." << endl;
        cout << "" << endl;

        setState(State::MapLoaded);
        mapLoaded();

    }
    else
    {
        cout << "You have not entered the right command." << endl;
        cout << "" << endl;

        gameEngineStart();
    }
}

// Will load the map chosen. 2 choices of command available
void GameEngine::mapLoaded()
{
    cout << "You are now in the " << state << " state of the Game menu." << endl;

    cout << "Please enter the following command :  --> " << endl;
    cout << "" << endl;
    cout << "loadmap" << endl;
    cout << "or" << endl;
    cout << "validatemap" << endl;
    getline(cin, userInput);
    cout << "" << endl;

    if (userInput == "validatemap")
    {
        cout << "Transitioning to the next state --> Map Validated..." << endl;
        cout << "" << endl;

        setState(State::MapValidated);
        mapValidated();
    }
    else if(userInput == "loadmap")
    {
        // Reloading the same state we are in.
        mapLoaded();
    }
    else
    {
        cout << "You have not entered the right command." << endl;
        cout << "" << endl;

        mapLoaded();
    }
}

// Validates the chosen map.
void GameEngine::mapValidated()
{
    cout << "You are now in the " << state << " state of the Game menu." << endl;

    cout << "Please enter the following command : addplayer --> " << endl;
    getline(cin, userInput);
    cout << "" << endl;

    if (userInput == "addplayer")
    {
        cout << "Transitioning to the next state --> Players Added..." << endl;
        cout << "" << endl;

        setState(State::PlayersAdded);
        playersAdded();
    }
    else
    {
        cout << "You have not entered the right command." << endl;
        cout << "" << endl;

        mapValidated();
    }
}

// Adds players as need and then assigns them a country. 2 choices of command
void GameEngine::playersAdded()
{
    cout << "You are now in the " << state << " state of the Game menu." << endl;

    cout << "Please enter the following command :  --> "<<endl;
    cout << "" << endl;
    cout << "addplayer" << endl;
    cout << "or" << endl;
    cout << "assigncountries" << endl;
    getline(cin, userInput);
    cout << "" << endl;

    if (userInput == "assigncountries")
    {
        cout << "Transitioning to the next state --> Assign Reinforcement..." << endl;
        cout << "" << endl;

        setState(State::AssignReinforcements);
        assignReinforcements();
    }
    else if (userInput =="addplayer")
    {
        // Re-loading the same state we are in
        playersAdded();
    }
    else
    {
        cout << "You have not entered the right command." << endl;
        cout << "" << endl;

        playersAdded();
    }
}

// Assigns reinforcements as needed
void GameEngine::assignReinforcements()
{
    cout << "You are now in the " << state << " state of the Game menu." << endl;

    cout << "Please enter the following command : issueorder --> " << endl;
    getline(cin, userInput);
    cout << "" << endl;

    if (userInput == "issueorder")
    {
        cout << "Transitioning to the next state --> Issue Orders..." << endl;
        cout << "" << endl;

        setState(State::IssueOrders);
        issueOrders();
    }
    else
    {
        cout << "You have not entered the right command." << endl;
        cout << "" << endl;

        assignReinforcements();
    }
}

// Issue orders. 2 choices of command available
void GameEngine::issueOrders()
{
    cout << "You are now in the " << state << " state of the Game menu." << endl;

    cout << "Please enter the following command :  --> " << endl;
    cout << "" << endl;
    cout << "issueorder" << endl;
    cout << "or" << endl;
    cout << "endissueorders" << endl;
    getline(cin, userInput);
    cout << "" << endl;

    if (userInput == "endissueorders")
    {
        cout << "Transitioning to the next state --> Execute Orders..." << endl;
        cout << "" << endl;

        setState(State::ExecuteOrders);
        executeOrders();
    }
    else if(userInput =="issueorder")
    {
        // Re-laoding the state we are in
        issueOrders();
    }
    else
    {
        cout << "You have not entered the right command." << endl;
        cout << "" << endl;

        issueOrders();
    }
}

// Execute the orders given. 3 choices of command available
void GameEngine::executeOrders()
{
    cout << "You are now in the " << state << " state of the Game menu." << endl;

    cout << "Please enter the following command :  --> "<< endl;
    cout << "" << endl;
    cout << "execorder" << endl;
    cout << "or" << endl;
    cout << "endexecorders" << endl;
    cout << "or" << endl;
    cout << "win" << endl;
    getline(cin, userInput);
    cout << "" << endl;

    if (userInput == "win")
    {
        cout << "Transitioning to the next state --> Win..." << endl;
        cout << "" << endl;

        setState(State::Win);
        win();
    }
    else if(userInput =="execorder")
    {
        // Re-loading the state we are in
        executeOrders();
    }
    else if (userInput =="endexecorders")
    {
        cout << "Transitioning to the previous state --> Assign Reinforcement..." << endl;
        cout << "" << endl;

        setState(State::AssignReinforcements);
        assignReinforcements();
    }
    else
    {
        cout << "You have not entered the right command." << endl;
        cout << "" << endl;

        executeOrders();
    }
}

// Helps determine if you end the game or want to play another one.
void GameEngine::win()
{
    cout << "You are now in the " << state << " state of the Game menu." << endl;

    cout << "Please enter the following command : --> " << endl;
    cout << "" << endl;
    cout << "play" << endl;
    cout << "or" << endl;
    cout << "end" << endl;
    getline(cin, userInput);
    cout << "" << endl;

    if (userInput == "end")
    {
        cout << "" << endl;
        cout << "The game has now ended. Thank you for playing";
    }
    else if (userInput == "play")
    {
        cout << "You are starting a new game..." << endl;
        cout << "Transitioning to the first state --> Start..." << endl;
        cout << "" << endl;

        setState(State::Start);
        gameEngineStart();
    }
    else
    {
        cout << "You have not entered the right command." << endl;
        cout << "" << endl;

        win();
    }
}
