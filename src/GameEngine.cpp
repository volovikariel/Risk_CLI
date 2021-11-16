#include "GameEngine.h"
#include "CommandProcessing.h"
#include "Map.h"
#include "Player.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>


// Global StateGraph object for queries about states and transitions
StateGraphInfo stateGraphInfo = StateGraphInfo();


/* --- GameEngine --- */

std::ostream& operator << (std::ostream& out, const GameEngine::State source)
{
    static const char* names[9] =
    {
        "Start",
        "MapLoaded",
        "MapValidated",
        "PlayersAdded",
        "AssignReinforcements",
        "IssueOrders",
        "ExecuteOrders",
        "Win",
        "Quit"
    };

    out << names[static_cast<size_t>(source)];
    return out;
}

std::ostream& operator << (std::ostream& out, const GameEngine::Transition source)
{
    static const char* names[11] =
    {
        "LoadMap",
        "ValidateMap",
        "AddPlayer",
        "GameStart",
        "IssueOrder",
        "IssueOrdersEnd",
        "ExecOrder",
        "ExecOrdersEnd",
        "Win",
        "Replay",
        "Quit"
    };

    out << names[static_cast<size_t>(source)];
    return out;
}

GameEngine::GameEngine():
    state(State::Start),
    map(new Map()),
    players(),
    eliminated()
{

}

GameEngine::~GameEngine()
{
    delete map;

    for (Player* player : players)
    {
        delete player;
    }
}

ostream& operator << (ostream& out, const GameEngine& source)
{
    out << "state: " << source.state;
    return out;
}

GameEngine::GameEngine(const GameEngine& other):
    state(other.state),
    map(other.map),
    players(other.players),
    eliminated(other.eliminated)
{

}

GameEngine& GameEngine::operator = (const GameEngine& other)
{
    state = other.state;
    map = other.map;
    players = other.players;
    eliminated = other.eliminated;
    return *this;
}

GameEngine::State GameEngine::getState() const
{
    return state;
}

const StateInfo& GameEngine::getStateInfo() const
{
    // Query the global state graph info
    return stateGraphInfo.getStateInfo(state);
}

bool GameEngine::transitionState(State state)
{
    // Check if the requested transition is valid
    if (getStateInfo().canDoState(state))
    {
        this->state = state;
        // Notify the observers of the GameEngine state change
        notify();
        return true;
    }
    else
    {
        return false;
    }
}

bool GameEngine::transitionState(Transition transition)
{
    // Check if the requested transition is valid, and retrieve the requested new state
    State newState;
    if (getStateInfo().canDoTransition(transition, newState))
    {
        this->state = newState;
        return true;
    }
    else
    {
        return false;
    }
}

Map& GameEngine::getMap()
{
    return *map;
}

vector<Player*>& GameEngine::getPlayers()
{
    return players;
}

bool GameEngine::executeCommand(Command& command)
{
    // TODO complete all cases

    switch (command.getType())
    {
        case Command::Type::LoadMap:
        {
            map->releaseAllocs();

            int errorLine;
            MapLoader mapLoader;
            Map::FormatError result = mapLoader.load(command.getArgument(), *map, errorLine);

            std::ostringstream stream;
            if (errorLine > 0)
            {
                stream << "Load error: " << result << " on line " << errorLine;
            }
            else
            {
                stream << "Load error: " << result;
            }
            stream << " for map " << map->name;

            command.saveEffect(stream.str());

            if (result != Map::FormatError::None)
            {
                return false;
            }

            return transitionState(Transition::LoadMap);
        }
        case Command::Type::ValidateMap:
        {
            Map::FormatError result = map->validate();

            std::ostringstream stream;
            stream << "Validation error: " << result << " for map " << map->name;

            command.saveEffect(stream.str());

            if (result != Map::FormatError::None)
            {
                return false;
            }

            return transitionState(Transition::ValidateMap);
        }
        case Command::Type::AddPlayer:
        {
            // TODO

            std::string effect = "Added player " + command.getArgument();
            command.saveEffect(effect);

            return transitionState(Transition::AddPlayer);
        }
        case Command::Type::GameStart:
        {
            // TODO

            command.saveEffect("Started game, performed initial setup");

            return transitionState(Transition::GameStart);
        }
        case Command::Type::Replay:
        {
            // TODO

            command.saveEffect("Replaying new game");

            return transitionState(Transition::Replay);
        }
        case Command::Type::Quit:
        {
            // TODO

            command.saveEffect("Quitting game");

            return transitionState(Transition::Quit);
        }
    }

    return false;
}

string GameEngine::stringToLog()
{
    // These are the states to which the State enum maps to
    string states[] = {
        "Start",
        "MapLoaded",
        "MapValidated",
        "PlayersAdded",
        "AssignReinforcements",
        "IssueOrders",
        "ExecuteOrders",
        "Win",
        "Quit",
        "NumStates"
    };
    return "GameEngine: State has been changed to '" + states[(int)this->getState()] + "'\n";
}

// Controls each game phase
void GameEngine::mainGameLoop() {
    // Removes players with no territories
    eliminatePlayer();
    // Check if more than one player
    while (eliminated.size() < this->getPlayers().size()-1) {
        GameEngine::reinforcementPhase();
        GameEngine::issueOrdersPhase();
        GameEngine::executeOrdersPhase();
    }
    // If game is won by a player provide replay or quit options
    if (this->transitionState(GameEngine::State::Win)) {
        cout << "Game Won, enter 'replay' to play again or 'quit' to stop playing" << endl;
        string input;
        cin >> input;
        while (this->transitionState(GameEngine::State::Win)) {
            if (input == "replay") {
                this->transitionState(GameEngine::Transition::Replay);
            } else if (input == "quit") {
                this->transitionState(GameEngine::Transition::Quit);
            } else {
                cout << "Invalid Command, try again." << endl;
                cin >> input;
            }
        }
    }
}

// Give players armies based on territories owns and bonus from continent
void GameEngine::reinforcementPhase() {

    this->transitionState(GameEngine::State::AssignReinforcements);

    int playerIndex = 0;
    for(auto i : players) {
        // Verify if player is not eliminated
        if (!isEliminated(i)) {
            // Add Armies based on territory owned divided by 3
            i->setPlayerArmies(i->getPlayerArmies() + floor(i->getPlayerTerritories().size() /3));

            // Check if player controls continent
            vector<int> numTerritoriesPerContinent(this->getMap().continents.size());
            for (auto j : i->getPlayerTerritories())
                numTerritoriesPerContinent[j->continentID-1] += 1;

            int index = 0;
            for (auto j : getMap().continents) {
                // If player controls continent add bonus
                if (j->territories.size() == numTerritoriesPerContinent[index]) {
                    i->setPlayerArmies(i->getPlayerArmies() + getMap().continents[index]->bonus);
                }
                index++;
                continue;
            }

            // Set minimum armies given to 3
            if (i->getPlayerArmies() < 3)
                i->setPlayerArmies(3);
            playerIndex ++;
        }
    }
}

// Issues orders to players based on the player's criteria
void GameEngine::issueOrdersPhase() {

    this->transitionState(GameEngine::State::IssueOrders);
    // Determines when to stop issuing orders
    while (keepIssuing()) {
        for (size_t i = 0; i < this->getPlayers().size(); i++) {
            // Verify if player is not eliminated
            if (!isEliminated(this->getPlayers().at(i))) {
                Order *currOrder = this->getPlayers().at(i)->issueOrder();
                if (currOrder != nullptr)
                    // Adds order to list
                    this->getPlayers().at(i)->getPlayerOrders()->addOrder(currOrder);
            }
        }
    }
}

// Executes orders from each player one by one
void GameEngine::executeOrdersPhase() {

    this->transitionState(GameEngine::State::ExecuteOrders);

    // Verify if theres more order to execute
    while (this->moreOrders()) {
        for (size_t i = 0; i < this->getPlayers().size(); i++) {
            // Verify if player is not eliminated
            if (!isEliminated(this->getPlayers().at(i))) {
                OrdersList *ol = this->getPlayers().at(i)->getPlayerOrders();
                vector<Order *> &orders = this->getPlayers().at(i)->getPlayerOrders()->getOrdersList();
                if (orders.size() > 0) {
                    Order *order = orders.front();
                    // Execute Deploy orders first
                    if (order->getType() == Order::Type::Deploy) {
                        cout << "Execute Deploy" << endl;
                        if (order->validate())
                            order->execute();
                        else
                            cout << "Order Invalid" << endl;
                        ol->remove(0);
                        // Executes Other order after all deploy order are executed
                    } else if (!moreDeploy()) { // TODO Need to modify moreDeploy method for checking deploy orders throughout OrderList since Reinforcement creates deploy orders in the middle
                        if (order->validate())
                            order->execute();
                        // Verify if player has won the game
                        if (this->getPlayers().at(i)->getPlayerTerritories().size() == this->getMap().territories.size()) {
                            this->transitionState(GameEngine::State::Win);
                            return;
                        } else
                            cout << "Order Invalid" << endl;
                        ol->remove(0);
                    }
                }
            }

        }
    }
}

// Determines how many orders are issue to player (returns false if 5 orders after deploy orders have been created)
bool GameEngine::keepIssuing() {
    for (auto p : this->getPlayers()) {
        if (p->getPlayerArmies() > 0) {
            return true;
        }
        // Create 5 more orders after deploy orders (Could be changed to a random number if preferred)
        if (p->getPlayerOrders()->getOrdersList().at(p->getPlayerOrders()->getOrdersList().size()-4)->getType() == Order::Type::Deploy) {
            return true;
        }
    }
    return false;
}

// Returns true if more deploy orders need to be processed TODO modify to search whole list for deploy order if its in the middle because of reinforcement card
bool GameEngine::moreDeploy() {
    for (auto p : this->getPlayers()) {
        if (p->getPlayerOrders()->getOrdersList().front()->getType() == Order::Type::Deploy) {
            return true;
        }
    }
    return false;
}

// Return true if players has more orders to be processed
bool GameEngine::moreOrders() {
    for (auto p : this->getPlayers()) {
        if (p->getPlayerOrders()->getOrdersList().size() > 0)
            return true;
    }
    return false;
}

// Removes players that have no owned territories
void GameEngine::eliminatePlayer() {
    for (auto p : this->getPlayers()) {
        if (p->getPlayerTerritories().size() == 0) {
            eliminated.push_back(p);
        }
    }
}

// Verify if a player is eliminated
bool GameEngine::isEliminated(Player* p) {
    for (auto ep : eliminated) {
        if (ep->getPlayerName() == p->getPlayerName()) {
            return true;
        }
    }
    return false;
}


/* --- StateInfo --- */

StateInfo::StateInfo():
    stateTransitions(),
    states(),
    transitions()
{

}

StateInfo::StateInfo(const vector<pair<GameEngine::State, GameEngine::Transition>>& stateTransitions)
{
    this->stateTransitions = stateTransitions;

    // Populate states and transitions using stateTransitions
    for (auto& pair : this->stateTransitions)
    {
        states.push_back(pair.first);
        transitions.push_back(pair.second);
    }
}

StateInfo::StateInfo(const StateInfo& other):
    stateTransitions(other.stateTransitions),
    states(other.states),
    transitions(other.transitions)
{

}

StateInfo::~StateInfo()
{

}

StateInfo& StateInfo::operator = (const StateInfo& other)
{
    this->stateTransitions = other.stateTransitions;
    this->states = other.states;
    this->transitions = other.transitions;
    return *this;
}

std::ostream& operator << (std::ostream& out, const StateInfo& source)
{
    for (auto& pair : source.stateTransitions)
    {
        out << pair.first << " (" << pair.second << ")" << endl;
    }
    return out;
}

const vector<pair<GameEngine::State, GameEngine::Transition>> StateInfo::getStateTransitions() const
{
    return stateTransitions;
}

const vector<GameEngine::State> StateInfo::getStates() const
{
    return states;
}

const vector<GameEngine::Transition> StateInfo::getTransitions() const
{
    return transitions;
}

bool StateInfo::canDoState(GameEngine::State state) const
{
    // Check if this state has the state as a direct connection
    return find(states.begin(), states.end(), state) != states.end();
}

bool StateInfo::canDoState(GameEngine::State state, GameEngine::Transition& result) const
{
    // Check if this state has the state as a direct connection,
    // and find the transition that leads to it
    for (auto& pair : stateTransitions)
    {
        if (pair.first == state)
        {
            result = pair.second;
            return true;
        }
    }
    return false;
}

bool StateInfo::canDoTransition(GameEngine::Transition transition) const
{
    // Check if this state has the transition
    return find(transitions.begin(), transitions.end(), transition) != transitions.end();
}

bool StateInfo::canDoTransition(GameEngine::Transition transition, GameEngine::State& result) const
{
    // Check if this state has the transition,
    // and find which new state it leads to
    for (auto& pair : stateTransitions)
    {
        if (pair.second == transition)
        {
            result = pair.first;
            return true;
        }
    }
    return false;
}




/* --- StateGraph --- */

StateGraphInfo::StateGraphInfo()
{
    size_t numStates = static_cast<size_t>(GameEngine::State::NumStates);
    states = vector<StateInfo*>(numStates);

    // Mirroring the connectivity of the graph that is described in the assignment instructions

    states[static_cast<size_t>(GameEngine::State::Start)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {
        make_pair(GameEngine::State::MapLoaded, GameEngine::Transition::LoadMap)
    });

    states[static_cast<size_t>(GameEngine::State::MapLoaded)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {
        make_pair(GameEngine::State::MapLoaded, GameEngine::Transition::LoadMap),
        make_pair(GameEngine::State::MapValidated, GameEngine::Transition::ValidateMap)
    });

    states[static_cast<size_t>(GameEngine::State::MapValidated)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {
        make_pair(GameEngine::State::PlayersAdded, GameEngine::Transition::AddPlayer)
    });

    states[static_cast<size_t>(GameEngine::State::PlayersAdded)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {
        make_pair(GameEngine::State::PlayersAdded, GameEngine::Transition::AddPlayer),
        make_pair(GameEngine::State::AssignReinforcements, GameEngine::Transition::GameStart)
    });

    states[static_cast<size_t>(GameEngine::State::AssignReinforcements)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {
        make_pair(GameEngine::State::IssueOrders, GameEngine::Transition::IssueOrder)
    });

    states[static_cast<size_t>(GameEngine::State::IssueOrders)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {
        make_pair(GameEngine::State::IssueOrders, GameEngine::Transition::IssueOrder),
        make_pair(GameEngine::State::ExecuteOrders, GameEngine::Transition::IssueOrdersEnd)
    });

    states[static_cast<size_t>(GameEngine::State::ExecuteOrders)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {
        make_pair(GameEngine::State::ExecuteOrders, GameEngine::Transition::ExecOrder),
        make_pair(GameEngine::State::AssignReinforcements, GameEngine::Transition::ExecOrdersEnd),
        make_pair(GameEngine::State::Win, GameEngine::Transition::Win)
    });

    states[static_cast<size_t>(GameEngine::State::Win)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {
        make_pair(GameEngine::State::Start, GameEngine::Transition::Replay),
        make_pair(GameEngine::State::Quit, GameEngine::Transition::Quit)
    });

    states[static_cast<size_t>(GameEngine::State::Quit)] = new StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>
    {

    });
}

StateGraphInfo::StateGraphInfo(const StateGraphInfo& other):
    states(other.states)
{

}

StateGraphInfo::~StateGraphInfo()
{
    for (StateInfo* stateInfo : states)
    {
        delete stateInfo;
    }
}

StateGraphInfo& StateGraphInfo::operator = (const StateGraphInfo& other)
{
    this->states = other.states;
    return *this;
}

std::ostream& operator << (std::ostream& out, const StateGraphInfo& source)
{
    for (size_t i = 0; i < source.states.size(); ++i)
    {
        GameEngine::State state = static_cast<GameEngine::State>(i);
        out << state << ":" << endl;
        out << source.states.at(i);
    }
    return out;
}

const StateInfo& StateGraphInfo::getStateInfo(GameEngine::State state) const
{
    size_t index = static_cast<size_t>(state);
    return *states.at(index);
}




/* --- StringUtils --- */

bool StringUtils::ToInteger(std::string& str, int& result)
{
    try
    {
        result = stoi(str);
    }
    catch (const invalid_argument&)
    {
        return false;
    }
    catch (const out_of_range&)
    {
        return false;
    }
    return true;
}

bool StringUtils::ToUnsignedInteger(std::string& str, int& result)
{
    int temp;
    if (StringUtils::ToInteger(str, temp) && temp >= 0)
    {
        result = temp;
        return true;
    }
    else
    {
        return false;
    }
}

std::string& StringUtils::ToLowerCase(std::string& str)
{
    for (auto& character : str)
    {
        character = tolower(character);
    }
    return str;
}

bool StringUtils::ToGameEngineTransition(std::string& str, GameEngine::Transition& result)
{
    std::string temp = str;
    StringUtils::ToLowerCase(temp);

    if (temp == "loadmap")
    {
        result = GameEngine::Transition::LoadMap;
        return true;
    }
    else if (temp == "validatemap")
    {
        result = GameEngine::Transition::ValidateMap;
        return true;
    }
    else if (temp == "addplayer")
    {
        result = GameEngine::Transition::AddPlayer;
        return true;
    }
    else if (temp == "gamestart")
    {
        result = GameEngine::Transition::GameStart;
        return true;
    }
    else if (temp == "issueorder")
    {
        result = GameEngine::Transition::IssueOrder;
        return true;
    }
    else if (temp == "issueordersend" || temp == "endissueorders")
    {
        result = GameEngine::Transition::IssueOrdersEnd;
        return true;
    }
    else if (temp == "execorder")
    {
        result = GameEngine::Transition::ExecOrder;
        return true;
    }
    else if (temp == "execordersend" || temp == "endexecorders")
    {
        result = GameEngine::Transition::ExecOrdersEnd;
        return true;
    }
    else if (temp == "win")
    {
        result = GameEngine::Transition::Win;
        return true;
    }
    else if (temp == "replay" || temp == "restart")
    {
        result = GameEngine::Transition::Replay;
        return true;
    }
    else if (temp == "quit" || temp == "exit")
    {
        result = GameEngine::Transition::Quit;
        return true;
    }

    return false;
}
