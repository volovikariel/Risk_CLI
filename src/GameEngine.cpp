#include "GameEngine.h"

#include <iostream>

// Global StateGraph object for queries about states and transitions
StateGraphInfo stateGraphInfo = StateGraphInfo();


// GameEngine

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
    state(State::Start)
{

}

GameEngine::~GameEngine()
{

}

ostream& operator << (ostream& out, const GameEngine& source)
{
    out << "state: " << source.state;
    return out;
}

GameEngine::GameEngine(const GameEngine& other):
    state(other.state)
{

}

GameEngine& GameEngine::operator = (const GameEngine& other)
{
    state = other.state;
    return *this;
}

GameEngine::State GameEngine::getState() const
{
    return state;
}

const StateInfo& GameEngine::getStateInfo() const
{
    return stateGraphInfo.getStateInfo(state);
}

bool GameEngine::transitionState(State state)
{
    if (getStateInfo().canDoState(state))
    {
        this->state = state;
        return true;
    }
    else
    {
        return false;
    }
}

bool GameEngine::transitionState(Transition transition)
{
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




// StateInfo

StateInfo::StateInfo():
    stateTransitions(),
    states(),
    transitions()
{

}

StateInfo::StateInfo(vector<pair<GameEngine::State, GameEngine::Transition>>& stateTransitions)
{
    this->stateTransitions = stateTransitions;

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
    return find(states.begin(), states.end(), state) != states.end();
}

bool StateInfo::canDoState(GameEngine::State state, GameEngine::Transition& result) const
{
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
    return find(transitions.begin(), transitions.end(), transition) != transitions.end();
}

bool StateInfo::canDoTransition(GameEngine::Transition transition, GameEngine::State& result) const
{
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




// StateGraph

StateGraphInfo::StateGraphInfo()
{
    size_t numStates = static_cast<size_t>(GameEngine::State::NumStates);
    states = vector<StateInfo*>(numStates);

    // Mirroring the connectivity of the graph

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




// StringUtils

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

void StringUtils::ToLowerCase(std::string& str)
{
    for (auto& character : str)
    {
        character = tolower(character);
    }
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
