#include "GameEngine.h"
#include "CommandProcessing.h"
#include "Map.h"
#include "Player.h"
#include "PlayerStrategies.h"

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
    static const char* names[12] =
    {
        "Tournament",
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
    alivePlayers(),
    eliminatedPlayers()
{
    neutralPlayer = new Player();
    neutralPlayer->setName("Neutral");
}

GameEngine::~GameEngine()
{
    cleanup(false);
}

void GameEngine::cleanup(bool prepareNewGame)
{
    if (prepareNewGame)
    {
        map->releaseAllocs();
        mainDeck.releaseAllocs();

        players.clear();
        alivePlayers.clear();
        eliminatedPlayers.clear();

        delete neutralPlayer;
        neutralPlayer = new Player();
        neutralPlayer->setName("Neutral");
    }
    else
    {
        delete map;
        delete neutralPlayer;
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
    neutralPlayer(other.neutralPlayer),
    alivePlayers(other.alivePlayers),
    eliminatedPlayers(other.eliminatedPlayers)
{

}

GameEngine& GameEngine::operator = (const GameEngine& other)
{
    state = other.state;
    map = other.map;
    players = other.players;
    neutralPlayer = other.neutralPlayer;
    alivePlayers = other.alivePlayers;
    eliminatedPlayers = other.eliminatedPlayers;
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

vector<Player*>& GameEngine::getAlivePlayers()
{
    return alivePlayers;
}

vector<Player*>& GameEngine::getEliminatedPlayers()
{
    return eliminatedPlayers;
}

Player& GameEngine::getNeutralPlayer()
{
    return *neutralPlayer;
}

bool GameEngine::addPlayer(Player& player, bool canRename)
{
    int nameIndex = 2;

    // Make sure the player name is unique
    for (const Player* existingPlayer : players)
    {
        if (player.getName() == existingPlayer->getName())
        {
            if (canRename)
            {
                player.setName(player.getName() + to_string(nameIndex));
                nameIndex++;
            }
            else
            {
                return false;
            }
        }
    }

    // Attach our observers
    for (Observer* observer : observers)
    {
        player.getOrders()->attach(*observer);
    }

    players.push_back(&player);
    alivePlayers.push_back(&player);

    return true;
}

bool GameEngine::start(string mapFilepath, vector<Player*>& players)
{
    // Cleanup memory from previous game
    cleanup(true);

    // Reset state
    state = State::Start;

    Command loadMap(Command::Type::LoadMap, mapFilepath);
    Command validateMap(Command::Type::ValidateMap);
    Command gameStart(Command::Type::GameStart);

    // Attach our observers
    for (Observer* observer : observers)
    {
        loadMap.attach(*observer);
        validateMap.attach(*observer);
        gameStart.attach(*observer);
    }

    bool success = true;

    success = success && executeCommand(loadMap);
    success = success && executeCommand(validateMap);

    for (Player* player : players)
    {
        success = success && addPlayer(*player, true);
        transition(Transition::AddPlayer);
    }

    success = success && executeCommand(gameStart);

    return success;
}

bool GameEngine::executeCommand(Command& command)
{
    switch (command.getType())
    {
        case Command::Type::Tournament:
        {
            TournamentCommandData* data = static_cast<TournamentCommandData*>(command.data);

            size_t numMaps = data->maps.size();
            if (numMaps < 1 || numMaps > 5)
            {
                std::ostringstream stream;
                stream << "Must have between 1 and 5 maps, got: " << numMaps;

                command.saveEffect(stream.str());
                return false;
            }

            size_t numStrategies = data->strategies.size();
            if (numStrategies < 2 || numStrategies > 4)
            {
                std::ostringstream stream;
                stream << "Must have between 2 and 4 strategies, got: " << numStrategies;

                command.saveEffect(stream.str());
                return false;
            }

            if (data->games < 1 || data->games > 5)
            {
                std::ostringstream stream;
                stream << "Must have between 1 and 5 games, got: " << data->games;

                command.saveEffect(stream.str());
                return false;
            }

            if (data->maxTurns < 10 || data->maxTurns > 50)
            {
                std::ostringstream stream;
                stream << "Must have between 10 and 50 max turns per game, got: " << data->maxTurns;

                command.saveEffect(stream.str());
                return false;
            }

            vector<string> winningStrategies;

            for (int game = 0; game < data->games; ++game)
            {
                for (const string& map : data->maps)
                {
                    // Make sure the map is unique
                    int mapCount = 0;
                    for (const string& otherMap : data->maps)
                    {
                        if (otherMap == map)
                        {
                            mapCount++;
                        }
                    }
                    if (mapCount > 1)
                    {
                        command.saveEffect("Duplicate map provided: " + map);
                        return false;
                    }

                    vector<Player*> roundPlayers;

                    // Assign player strategy
                    for (const string& strategy : data->strategies)
                    {
                        Player* player = new Player();
                        roundPlayers.push_back(player);

                        if (strategy == "aggressive")
                        {
                            AggressivePlayerStrategy* aggressiveStrategy = new AggressivePlayerStrategy(*player);
                            player->setPlayerStrategy(*aggressiveStrategy);
                        }
                        else if (strategy == "benevolent")
                        {
                            BenevolentPlayerStrategy* benevolentStrategy = new BenevolentPlayerStrategy(*player);
                            player->setPlayerStrategy(*benevolentStrategy);
                        }
                        else if (strategy == "neutral")
                        {
                            NeutralPlayerStrategy* neutralStrategy = new NeutralPlayerStrategy(*player);
                            player->setPlayerStrategy(*neutralStrategy);
                        }
                        else if (strategy == "cheater")
                        {
                            CheaterPlayerStrategy* cheaterStrategy = new CheaterPlayerStrategy(*player);
                            player->setPlayerStrategy(*cheaterStrategy);
                        }
                        else
                        {
                            for (Player* player : roundPlayers)
                            {
                                delete player;
                            }
                            command.saveEffect("Unknown player strategy: " + strategy);
                            state = State::Start;
                            return false;
                        }

                        player->setName(StringUtils::strategyToString(player->getPlayerStrategy()));

                        // Make sure the player strategy is unique
                        for (const Player* otherPlayer : roundPlayers)
                        {
                            if (otherPlayer != player && otherPlayer->getName() == player->getName())
                            {
                                for (Player* player : roundPlayers)
                                {
                                    delete player;
                                }
                                command.saveEffect("Duplicate player strategies provided: " + strategy);
                                state = State::Start;
                                return false;
                            }
                        }
                    }

                    // Load map, set players, initial setup
                    if (start(map, roundPlayers))
                    {
                        string winningStrategy = "Draw";

                        // Run until we have a draw at maxTurns or a winner
                        for (int turn = 0; turn < data->maxTurns && alivePlayers.size() > 1; ++turn)
                        {
                            executeTurn();
                        }

                        // Reset state
                        state = State::Start;

                        // Determine winner
                        if (alivePlayers.size() == 1)
                        {
                            std::ostringstream stream;
                            stream << StringUtils::strategyToString(alivePlayers.front()->getPlayerStrategy());
                            winningStrategy = stream.str();
                        }

                        winningStrategies.push_back(winningStrategy);
                    }
                    else
                    {
                        for (Player* player : roundPlayers)
                        {
                            delete player;
                        }
                        command.saveEffect("Couldn't start round");
                        state = State::Start;
                        return false;
                    }

                    for (Player* player : roundPlayers)
                    {
                        delete player;
                    }
                }
            }

            // Create a formatted results table
            std::ostringstream stream;
            stream << endl << endl;
            stream << "Tournament Mode:" << (*data) << endl << endl;
            stream << "Results:" << endl;
            stream << StringUtils::SpacePad("", 16);
            for (int game = 0; game < data->games; ++game)
            {
                std::string gameLabel = "Game " + std::to_string(game + 1);
                stream << StringUtils::SpacePad(gameLabel, 16);
            }
            stream << endl;
            for (size_t map = 0; map < data->maps.size(); ++map)
            {
                std::string mapName = StringUtils::Filename(data->maps.at(map));
                stream << StringUtils::SpacePad(mapName, 16);

                for (int game = 0; game < data->games; ++game)
                {
                    size_t idx = game * data->maps.size() + map;
                    std::string& winningStrategy = winningStrategies.at(idx);
                    stream << StringUtils::SpacePad(winningStrategy, 16);
                }
                stream << endl;
            }

            command.saveEffect(stream.str());

            return true;
        }
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
                if (player->getName() == newPlayerName)
                {
                    std::ostringstream stream;
                    stream << "There is already a player with the name: " << newPlayerName;

                    command.saveEffect(stream.str());
                    return false;
                }
            }

            Player* player = new Player();
            player->setName(newPlayerName);

            bool success = addPlayer(*player, false);

            if (success)
            {
                string effect = "Added player " + newPlayerName;
                command.saveEffect(effect);
            }
            else
            {
                string effect = "Failed to add player " + newPlayerName;
                command.saveEffect(effect);
            }

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

            for (size_t i = 0; i < map->territories.size(); i++)
            {
                territoryRand = rand() % copyTerritories.size(); // Generate the random index of the territory
                Territory* territory = copyTerritories.at(territoryRand); // Store the specific territory
                Player* playerToAssign = players.at(i % players.size());
                territory->player = playerToAssign;
                playerToAssign->addTerritory(territory);
                copyTerritories.erase(copyTerritories.begin() + territoryRand); // Will delete the territory assigned from the temporary vector
            }

            // Shuffle in a random manner the order of players
            std::random_shuffle(players.begin(), players.end());

            // Gives the players 50 armies
            for (Player* player : players)
            {
                player->setArmies(50);
            }

            // Initialize deck with 50 randomly shuffled cards (of even card type distribution)
            mainDeck.addRandomCards(50);

            //Allows players to draw 2 cards from the deck
            for (Player* player : players)
            {
                Hand& hand = *player->getCards();
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
            // Cleanup memory
            cleanup(true);

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

// Runs the game until only one player is left
void GameEngine::mainGameLoop()
{
    // Check if more than one player remaining
    while (alivePlayers.size() > 1)
    {
        executeTurn();
    }

    transition(GameEngine::State::Win);
}

// Executes a single turn of the game
void GameEngine::executeTurn()
{
    GameEngine::reinforcementPhase();
    GameEngine::issueOrdersPhase();
    GameEngine::executeOrdersPhase();

    // Give players who conquered a territory last turn a card
    for (Player* player : alivePlayers)
    {
        bool hasMaxCardsInHand = player->getCards()->getCards().size() >= 6;
        if (player->hasConqueredThisTurn && !hasMaxCardsInHand)
        {
            player->hasConqueredThisTurn = false;
            player->getCards()->addCard(*mainDeck.draw());
        }
    }

    // Clear diplomatic ties from last turn
    for (Player* player : alivePlayers)
    {
        player->clearUnattackable();
    }

    // Removes players with no territories
    eliminatePlayers();

    // Increment turn counter
    turnIndex++;
}

// Give players armies based on territories owns and bonus from continent
void GameEngine::reinforcementPhase()
{
    transition(GameEngine::State::AssignReinforcements);

    for (auto player : alivePlayers)
    {
        // Add Armies based on territory owned divided by 3
        int armiesToAdd = static_cast<int>(player->getTerritories().size()) / 3;

        // Check if player controls continents
        vector<int> numTerritoriesPerContinent(map->continents.size());
        for (auto territory : player->getTerritories())
        {
            numTerritoriesPerContinent[territory->continentID - 1]++;
        }
        for (auto continent : map->continents)
        {
            // If player controls continent add bonus
            if (continent->territories.size() == numTerritoriesPerContinent[continent->ID - 1])
            {
                armiesToAdd += continent->bonus;
            }
        }

        // Set minimum armies given to 3
        armiesToAdd = max(armiesToAdd, 3);

        // Set player armies
        player->setArmies(player->getArmies() + armiesToAdd);
    }
}

// Issues orders to players based on the player's criteria
void GameEngine::issueOrdersPhase()
{
    this->transition(GameEngine::State::IssueOrders);

    // Determines when to stop issuing orders
    while (true)
    {
        int skipCount = 0;

        for (Player* player : alivePlayers)
        {
            Order* currOrder = player->issueOrder(*this);
            if (currOrder == nullptr)
            {
                skipCount++;
            }
            else
            {
                player->getOrders()->addOrder(currOrder);
            }
        }

        // All players signaled they have no more orders to issue
        if (skipCount == alivePlayers.size())
        {
            break;
        }
    }
}

// Executes orders from each player one by one
void GameEngine::executeOrdersPhase()
{
    this->transition(GameEngine::State::ExecuteOrders);

    // Execute deploy orders from each player (round-robin)
    while (true)
    {
        int skipCount = 0;

        for (Player* player : alivePlayers)
        {
            OrdersList* ordersList = player->getOrders();
            vector<Order*>& orders = ordersList->getOrdersList();

            if (orders.size() == 0)
            {
                skipCount++;
            }
            else
            {
                Order* order = orders.front();

                if (order->getType() == Order::Type::Deploy)
                {
                    order->execute();
                    ordersList->remove(0);
                }
                else
                {
                    skipCount++;
                }
            }
        }

        // All players signaled they have no more deploy orders
        if (skipCount == alivePlayers.size())
        {
            break;
        }
    }

    // Execute orders from each player (round-robin)
    while (true)
    {
        int skipCount = 0;

        for (Player* player : alivePlayers)
        {
            OrdersList* ordersList = player->getOrders();
            vector<Order*>& orders = ordersList->getOrdersList();

            if (orders.size() == 0)
            {
                skipCount++;
            }
            else
            {
                Order* order = orders.front();
                order->execute();
                ordersList->remove(0);
            }
        }

        // All players signaled they have no more orders
        if (skipCount == alivePlayers.size())
        {
            break;
        }
    }
}

// Removes players that have no owned territories
void GameEngine::eliminatePlayers()
{
    vector<Player*>::iterator i = alivePlayers.begin();
    while (i != alivePlayers.end())
    {
        Player* player = *i;

        bool isAlive = player->getTerritories().size() > 0;
        if (isAlive)
        {
            ++i;
        }
        else
        {
            eliminatedPlayers.push_back(player);
            i = alivePlayers.erase(i);
        }
    }
}

int GameEngine::getTurnIndex() const
{
    return turnIndex;
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
        make_pair(GameEngine::State::Start, GameEngine::Transition::Tournament),
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

std::string StringUtils::SpacePad(const std::string& str, size_t numSpaces)
{
    std::string temp = str;

    size_t toAdd = numSpaces - str.size();
    for (size_t i = 0; i < toAdd; ++i)
    {
        temp += ' ';
    }

    return temp;
}

std::string StringUtils::Filename(const std::string& filepath)
{
    size_t sepIdx = 0;
    size_t dotIdx = filepath.size();
    for (size_t i = 0; i < filepath.size(); ++i)
    {
        const char& currentChar = filepath.at(i);
        if (currentChar == '/' || currentChar == '\\' || currentChar == ':')
        {
            sepIdx = i + 1;
        }
        if (currentChar == '.')
        {
            dotIdx = i;
        }
    }
    return filepath.substr(sepIdx, dotIdx - sepIdx);
}

bool StringUtils::ToGameEngineTransition(std::string& str, GameEngine::Transition& result)
{
    std::string temp = str;
    StringUtils::ToLowerCase(temp);

    if (temp == "tournament")
    {
        result = GameEngine::Transition::Tournament;
        return true;
    }
    else if (temp == "loadmap")
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
