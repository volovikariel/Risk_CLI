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
    neutralPlayer = new Player();
    neutralPlayer->setPlayerName("Neutral");
}

GameEngine::~GameEngine()
{
    delete map;

    for (Player* player : players)
    {
        delete player;
    }

    delete neutralPlayer;
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
    neutralPlayer(other.neutralPlayer),
    eliminated(other.eliminated)
{

}

GameEngine& GameEngine::operator = (const GameEngine& other)
{
    state = other.state;
    map = other.map;
    players = other.players;
    neutralPlayer = other.neutralPlayer;
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

bool GameEngine::transition(State state)
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

bool GameEngine::transition(Transition transition)
{
    // Check if the requested transition is valid, and retrieve the requested new state
    State newState;
    if (getStateInfo().canDoTransition(transition, newState))
    {
        this->state = newState;
        // Notify the observers of the GameEngine state change
        notify();
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

Player& GameEngine::getNeutralPlayer()
{
    return *neutralPlayer;
}

bool tryExecuteCommand(GameEngine& gameEngine, CommandProcessor& commandProcessor, Command& command)
{
    // Check that the requested state transition is valid
    if (!commandProcessor.validate(command))
    {
        // The requested state transition is invalid

        // Get game engine's current state
        GameEngine::State state = gameEngine.getState();

        // Resolve command to transition mapping
        GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command.getType());

        // Set detailed explanation for command status
        std::ostringstream stream;
        stream << "The current state is " << state << " which has no transition " << requestedTransition;
        command.saveEffect(stream.str());

        // Print command
        std::cout << std::endl;
        std::cout << "Invalid command in the current game engine state:" << std::endl;
        std::cout << command << std::endl;

        return false;
    }
    else
    {
        // The requested state transition is valid

        // Resolve command to transition mapping
        GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command.getType());

        // Execute command (if succesful, will also apply the state transition)
        // Details are saved to the command's effect string
        bool success = gameEngine.executeCommand(command);

        // Print execution result
        std::cout << std::endl;
        if (success)
        {
            std::cout << "Command execution succeeded:" << std::endl;
        }
        else
        {
            std::cout << "Command execution failed:" << std::endl;
        }
        std::cout << command << std::endl;

        return success;
    }
}

void GameEngine::startupPhase()
{
    // Create console input command processor
    CommandProcessor commandProcessor(*this);

    // Keep reading commands from input
    while (true)
    {
        // Read command from input until we obtain a well-formed command
        Command* command = nullptr;
        while (command == nullptr || command->getType() == Command::Type::Invalid)
        {
            command = commandProcessor.getCommand();
        }

        bool success = tryExecuteCommand(*this, commandProcessor, *command);

        // Setup is completed
        if (success && command->getType() == Command::Type::GameStart)
        {
            break;
        }
    }
}

bool GameEngine::executeCommand(Command& command)
{
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

            return transition(Transition::LoadMap);
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

            return transition(Transition::ValidateMap);
        }
        case Command::Type::AddPlayer:
        {
            const std::string& newPlayerName = command.getArgument();

            // Make sure the player name is unique
            for (const Player* player : players)
            {
                if (player->getPlayerName() == newPlayerName)
                {
                    std::ostringstream stream;
                    stream << "There is already a player with the name: " << newPlayerName;

                    command.saveEffect(stream.str());
                    return false;
                }
            }

            Player* player = new Player();
            player->setPlayerName(newPlayerName);

            players.push_back(player);
            string effect = "Added player " + newPlayerName;
            command.saveEffect(effect);

            return transition(Transition::AddPlayer);
        }
        case Command::Type::GameStart:
        {
            // Validate number of players
            if (players.size() < 2 || players.size() > 6)
            {
                std::ostringstream stream;
                stream << "Number of players must be between 2 and 6, currently: " << players.size();

                command.saveEffect(stream.str());
                return false;
            }

            // Will help assign a random territory to a player
            vector<Territory*> copyTerritories = map->territories;
            int territoryRand; // Will store a random index to a territory

            for (int i = 0; i < map->territories.size(); i++)
            {
                territoryRand = rand() % copyTerritories.size(); // Generate the random index of the territory
                Territory* territory = copyTerritories.at(territoryRand); // Store the specific territory
                Player* playerToAssign = players[i % players.size()];
                territory->player = playerToAssign;
                playerToAssign->addPlayerTerritory(territory);
                copyTerritories.erase(copyTerritories.begin() + territoryRand); // Will delete the territory assigned from the temporary vector
            }

            // Shuffle in a random manner the order of players
            std::random_shuffle(players.begin(), players.end());

            // Gives the players 50 armies
            for (Player* player : players)
            {
                player->setPlayerArmies(50);
            }

            // Initialize deck with 50 randomly shuffled cards (of even card type distribution)
            mainDeck.addRandomCards(50);

            //Allows players to draw 2 cards from the deck
            for (Player* player : players)
            {
                Hand& hand = *player->getPlayerCards();
                for (int i = 0; i < 2; i++)
                {
                    hand.addCard(*mainDeck.draw());
                }
            }

            command.saveEffect("Started game and performed initial setup");

            return transition(Transition::GameStart);
        }
        case Command::Type::Replay:
        {
            // TODO

            command.saveEffect("Replaying new game");

            return transition(Transition::Replay);
        }
        case Command::Type::Quit:
        {
            exit(0);

            command.saveEffect("Quitting game");

            return transition(Transition::Quit);
        }
    }

    return false;
}

string GameEngine::stringToLog()
{
    std::ostringstream stream;
    stream << "GameEngine: State has been changed to '" << state << "'";
    return stream.str();
}

// Controls each game phase
void GameEngine::mainGameLoop()
{
    // Check if more than one player remaining
    while (eliminated.size() < getPlayers().size() - 1)
    {
        GameEngine::reinforcementPhase();
        GameEngine::issueOrdersPhase();
        GameEngine::executeOrdersPhase();

        // Give players who conquered a territory last turn a card
        for (Player* player : players)
        {
            if (player->hasConqueredThisTurn)
            {
                player->hasConqueredThisTurn = false;
                player->getPlayerCards()->addCard(*mainDeck.draw());
            }
        }

        // Clear diplomatic ties from last turn
        for (Player* player : players)
        {
            player->clearUnattackable();
        }

        // Removes players with no territories
        eliminatePlayers();
    }

    // Will allow player to choose between quit and replay
    transition(GameEngine::State::Win);
    startupPhase();
}

// Give players armies based on territories owns and bonus from continent
void GameEngine::reinforcementPhase()
{
    transition(GameEngine::State::AssignReinforcements);

    for (auto player : players)
    {
        // Verify if player is not eliminated
        if (!isEliminated(player))
        {
            // Add Armies based on territory owned divided by 3
            player->setPlayerArmies(player->getPlayerArmies() + floor(player->getPlayerTerritories().size() / 3));

            // Check if player controls continents
            vector<int> numTerritoriesPerContinent(map->continents.size());
            for (auto territory : player->getPlayerTerritories())
            {
                numTerritoriesPerContinent[territory->continentID - 1]++;
            }

            for (auto continent : map->continents)
            {
                // If player controls continent add bonus
                if (continent->territories.size() == numTerritoriesPerContinent[continent->ID - 1])
                {
                    player->setPlayerArmies(player->getPlayerArmies() + continent->bonus);
                }
            }

            // Set minimum armies given to 3
            if (player->getPlayerArmies() < 3)
            {
                player->setPlayerArmies(3);
            }
        }
    }
}

// Issues orders to players based on the player's criteria
void GameEngine::issueOrdersPhase()
{
    this->transition(GameEngine::State::IssueOrders);

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

    this->transition(GameEngine::State::ExecuteOrders);

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
                            this->transition(GameEngine::State::Win);
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
void GameEngine::eliminatePlayers()
{
    for (auto p : this->getPlayers())
    {
        if (p->getPlayerTerritories().size() == 0)
        {
            eliminated.push_back(p);
        }
    }
}

// Verify if a player is eliminated
bool GameEngine::isEliminated(Player* p)
{
    for (auto ep : eliminated)
    {
        if (ep->getPlayerName() == p->getPlayerName())
        {
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
