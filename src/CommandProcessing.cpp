#include "CommandProcessing.h"
#include "Map.h"

#include <algorithm>
#include <iostream>
#include <sstream>

/* --- TournamentCommandData --- */

TournamentCommandData::TournamentCommandData():
    maps(),
    strategies(),
    games(0),
    maxTurns(0)
{

}

TournamentCommandData::TournamentCommandData(TournamentCommandData& other):
    maps(other.maps),
    strategies(other.strategies),
    games(other.games),
    maxTurns(other.maxTurns)
{

}

TournamentCommandData& TournamentCommandData::operator = (const TournamentCommandData& other)
{
    this->maps = other.maps;
    this->strategies = other.strategies;
    this->games = other.games;
    this->maxTurns = other.maxTurns;
    return *this;
}

std::ostream& operator << (std::ostream& out, const TournamentCommandData& source)
{
    out << "-M ";
    for (const std::string& map : source.maps)
    {
        out << map << " ";
    }
    out << "-P ";
    for (const std::string& strategy : source.strategies)
    {
        out << strategy << " ";
    }
    out << "-G " << source.games << " -D " << source.maxTurns;
    return out;
}

bool TournamentCommandData::parseStream(std::istream& in)
{
    std::string word;
    in >> word;

    if (word != "-M" || !in.good())
    {
        return false;
    }

    while (in.good())
    {
        in >> word;
        if (word.front() == '-')
            break;
        maps.push_back(word);
    }

    if (word != "-P" || !in.good())
    {
        return false;
    }

    while (in.good())
    {
        in >> word;
        if (word.front() == '-')
            break;
        strategies.push_back(word);
    }

    for (std::string& strategy : strategies)
    {
        StringUtils::ToLowerCase(strategy);
    }

    if (word != "-G" || !in.good())
    {
        return false;
    }

    in >> games;
    in >> word;

    if (word != "-D" || !in.good())
    {
        return false;
    }

    in >> maxTurns;

    return true;
}


/* --- Command --- */

std::ostream& operator << (std::ostream& out, const Command::Type source)
{
    static const char* names[9] =
    {
        "Tournament",
        "LoadMap",
        "ValidateMap",
        "AddPlayer",
        "GameStart",
        "Replay",
        "Quit",
        "NumTypes",
        "Invalid"
    };

    out << names[static_cast<size_t>(source)];
    return out;
}

Command::Command():
    type(),
    argument(),
    effect(),
    data(nullptr)
{

}

Command::Command(Type type):
    type(type),
    argument(),
    effect(),
    data(nullptr)
{

}

Command::Command(Type type, std::string& argument):
    type(type),
    argument(argument),
    effect(),
    data(nullptr)
{

}

Command::Command(const Command& other):
    type(other.type),
    argument(other.argument),
    effect(other.effect),
    data(other.data)
{

}

Command::~Command()
{
    if (data != nullptr)
    {
        delete reinterpret_cast<TournamentCommandData*>(data);
    }
}

Command& Command::operator = (const Command& other)
{
    this->type = other.type;
    this->argument = other.argument;
    this->effect = other.effect;
    this->data = other.data;
    return *this;
}

std::ostream& operator << (std::ostream& out, const Command& source)
{
    out << "Command [" << source.type << "]";
    if (source.argument.length() > 0)
    {
        out << "[" << source.argument << "]";
    }
    if (source.effect.length() > 0)
    {
        out << ": " << source.effect;
    }
    return out;
}

Command::Type Command::getType() const
{
    return type;
}

const std::string& Command::getArgument() const
{
    return argument;
}

void Command::saveEffect(const std::string& description)
{
    effect = description;
    // Notify the observers that the Command object has changed
    notify();
}

void Command::saveEffect(const char* description)
{
    effect = description;
    // Notify the observers that the Command object has changed
    notify();
}

string Command::stringToLog()
{
    std::ostringstream stream;
    stream << "Command executed: " << *this;
    return stream.str();
}


/* --- CommandProcessor --- */

CommandProcessor::CommandProcessor():
    commands(),
    gameEngine()
{

}

CommandProcessor::CommandProcessor(GameEngine& gameEngine):
    commands(),
    gameEngine(&gameEngine)
{

}

CommandProcessor::CommandProcessor(const CommandProcessor& other):
    commands(other.commands),
    gameEngine(other.gameEngine)
{

}

CommandProcessor::~CommandProcessor()
{
    for (Command* command : commands)
    {
        delete command;
    }
}

CommandProcessor& CommandProcessor::operator = (const CommandProcessor& other)
{
    this->commands = other.commands;
    this->gameEngine = other.gameEngine;
    return *this;
}

std::ostream& operator << (std::ostream& out, const CommandProcessor& source)
{
    for (Command* command : source.commands)
    {
        out << command << std::endl;
    }
    return out;
}

Command* CommandProcessor::getCommand()
{
    Command* command = readCommand();
    if (command != nullptr)
    {
        saveCommand(*command);

        for (Observer* observer : observers)
        {
            command->attach(*observer);
        }
    }
    return command;
}

bool CommandProcessor::validate(Command& command)
{
    // Resolve Command::Type to GameEngine::Transition mapping
    GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command.getType());

    // Check if the requested transition can be done from the current GameEngine::State
    return gameEngine->getStateInfo().canDoTransition(requestedTransition);
}

Command* CommandProcessor::readCommand()
{
    // Output available commands and current state
    std::cout << std::endl;
    std::cout << "Input one of the following commands [current state is " << gameEngine->getState() << "]:" << std::endl;

    auto& possibleTransitions = gameEngine->getStateInfo().getTransitions();
    size_t numCommands = static_cast<size_t>(Command::Type::NumTypes);
    for (size_t i = 0; i < numCommands; ++i)
    {
        Command::Type commandType = static_cast<Command::Type>(i);
        GameEngine::Transition transition = CommandProcessingUtils::commandToTransition(commandType);

        // Transform to lowercase for familiarity
        std::ostringstream stream;
        stream << transition;
        std::string transitionName = stream.str();
        StringUtils::ToLowerCase(transitionName);

        // Wrap valid transitions with [] brackets
        bool isValidContext = std::find(possibleTransitions.begin(), possibleTransitions.end(), transition) != possibleTransitions.end();

        if (isValidContext)
        {
            std::cout << "[";
        }
        std::cout << transitionName;

        // Include argument hints for commands that require them
        if (transition == GameEngine::Transition::LoadMap)
        {
            std::cout << " <mapfile>";
        }
        else if (transition == GameEngine::Transition::AddPlayer)
        {
            std::cout << " <playername>";
        }

        if (isValidContext)
        {
            std::cout << "]";
        }

        if (i != numCommands - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    // Get console line input
    std::string line;
    std::getline(std::cin, line);

    if (line.length() == 0)
    {
        return nullptr;
    }

    std::istringstream lineStream = std::istringstream(line);

    std::string command;
    std::string argument;

    // Parse the command word
    lineStream >> command;
    if (lineStream.fail())
    {
        return nullptr;
    }

    // Resolve the corresponding GameEngine::Transition
    GameEngine::Transition transition;
    if (!StringUtils::ToGameEngineTransition(command, transition))
    {
        std::cout << "Unknown command" << std::endl;
        return new Command(Command::Type::Invalid, line);
    }
    // Resolve the GameEngine::Transition to Command::Type mapping
    Command::Type commandType;
    if (!CommandProcessingUtils::transitionToCommand(transition, commandType))
    {
        std::cout << "Unknown command" << std::endl;
        return new Command(Command::Type::Invalid, line);
    }

    if (transition == GameEngine::Transition::Tournament)
    {
        TournamentCommandData* data = new TournamentCommandData();

        bool success = data->parseStream(lineStream);
        if (success)
        {
            Command* command = new Command(commandType);
            command->data = data;
            return command;
        }
        else
        {
            std::cout << "Invalid arguments. Format is: tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>" << std::endl;
            delete data;
            return new Command(Command::Type::Invalid, line);
        }
    }
    else if (transition == GameEngine::Transition::LoadMap || transition == GameEngine::Transition::AddPlayer)
    {
        // Parse the required argument
        lineStream >> argument;
        if (lineStream.fail())
        {
            std::cout << "Missing the argument for this command" << std::endl;
            return new Command(Command::Type::Invalid, line);
        }

        // Create the command
        return new Command(commandType, argument);
    }
    else
    {
        // Create the command
        return new Command(commandType);
    }
}

void CommandProcessor::saveCommand(Command& command)
{
    commands.push_back(&command);
    // Notify the observers that the CommandProcessor object has changed
    notify();
}

string CommandProcessor::stringToLog()
{
    Command* lastCommand = commands.back();

    std::ostringstream stream;
    stream << "CommandProcessor saved command: " << *lastCommand;
    return stream.str();
}


/* --- FileLineReader --- */

FileLineReader::FileLineReader():
    filepath(),
    filestream()
{

}

FileLineReader::FileLineReader(const std::string& filepath):
    filepath(filepath),
    filestream(filepath)
{

}

FileLineReader::FileLineReader(const FileLineReader& other):
    filepath(other.filepath)
{

}

FileLineReader::~FileLineReader()
{

}

FileLineReader& FileLineReader::operator = (const FileLineReader& other)
{
    filepath = other.filepath;
    return *this;
}

std::ostream& operator << (std::ostream& out, const FileLineReader& source)
{
    out << source.filepath;
    return out;
}

std::string FileLineReader::readLineFromFile()
{
    // Check that the file exists and still readable
    if (!filestream.good())
    {
        return "";
    }

    // Read one line
    std::string line;
    std::getline(filestream, line);

    return line;
}

bool FileLineReader::good() const
{
    return filestream.good();
}




/* --- FileCommandProcessorAdapter --- */

FileCommandProcessorAdapter::FileCommandProcessorAdapter():
    CommandProcessor(),
    fileLineReader(nullptr)
{

}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(GameEngine& gameEngine, FileLineReader& fileLineReader):
    CommandProcessor(gameEngine),
    fileLineReader(&fileLineReader)
{

}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other):
    CommandProcessor(other),
    fileLineReader(other.fileLineReader)
{

}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{

}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator = (const FileCommandProcessorAdapter& other)
{
    CommandProcessor::operator = (other);
    fileLineReader = other.fileLineReader;
    return *this;
}

std::ostream& operator << (std::ostream& out, const FileCommandProcessorAdapter& source)
{
    out << "Reading file: " << source.fileLineReader << std::endl;
    for (Command* command : source.commands)
    {
        out << command << std::endl;
    }
    return out;
}

Command* FileCommandProcessorAdapter::readCommand()
{
    // Read one line
    std::string line = fileLineReader->readLineFromFile();

    // Skip empty line
    if (line.length() <= 0)
    {
        return nullptr;
    }

    std::istringstream lineStream = std::istringstream(line);

    std::string command;
    std::string argument;

    // Parse the command word
    lineStream >> command;
    if (lineStream.fail())
    {
        return nullptr;
    }

    // Resolve the corresponding GameEngine::Transition
    GameEngine::Transition transition;
    if (!StringUtils::ToGameEngineTransition(command, transition))
    {
        return new Command(Command::Type::Invalid, line);
    }
    // Resolve the GameEngine::Transition to Command::Type mapping
    Command::Type commandType;
    if (!CommandProcessingUtils::transitionToCommand(transition, commandType))
    {
        return new Command(Command::Type::Invalid, line);
    }

    if (transition == GameEngine::Transition::Tournament)
    {
        TournamentCommandData* data = new TournamentCommandData();

        bool success = data->parseStream(lineStream);
        if (success)
        {
            Command* command = new Command(commandType);
            command->data = data;
            return command;
        }
        else
        {
            delete data;
            return new Command(Command::Type::Invalid, line);
        }
    }
    else if (transition == GameEngine::Transition::LoadMap || transition == GameEngine::Transition::AddPlayer)
    {
        // Parse the required argument
        lineStream >> argument;
        if (lineStream.fail())
        {
            return new Command(Command::Type::Invalid, line);
        }

        // Create the command
        return new Command(commandType, argument);
    }
    else
    {
        // Create the command
        return new Command(commandType);
    }
}

bool FileCommandProcessorAdapter::good() const
{
    return fileLineReader->good();
}




/* --- CommandProcessingUtils --- */

GameEngine::Transition CommandProcessingUtils::commandToTransition(Command::Type type)
{
    switch (type)
    {
    case Command::Type::Tournament:
        return GameEngine::Transition::Tournament;
    case Command::Type::LoadMap:
        return GameEngine::Transition::LoadMap;
    case Command::Type::ValidateMap:
        return GameEngine::Transition::ValidateMap;
    case Command::Type::AddPlayer:
        return GameEngine::Transition::AddPlayer;
    case Command::Type::GameStart:
        return GameEngine::Transition::GameStart;
    case Command::Type::Replay:
        return GameEngine::Transition::Replay;
    case Command::Type::Quit:
        return GameEngine::Transition::Quit;
    default:
        return GameEngine::Transition::Quit;
    }
}

bool CommandProcessingUtils::transitionToCommand(GameEngine::Transition transition, Command::Type& result)
{
    switch (transition)
    {
    case GameEngine::Transition::Tournament:
        result = Command::Type::Tournament;
        break;
    case GameEngine::Transition::LoadMap:
        result = Command::Type::LoadMap;
        break;
    case GameEngine::Transition::ValidateMap:
        result = Command::Type::ValidateMap;
        break;
    case GameEngine::Transition::AddPlayer:
        result = Command::Type::AddPlayer;
        break;
    case GameEngine::Transition::GameStart:
        result = Command::Type::GameStart;
        break;
    case GameEngine::Transition::Replay:
        result = Command::Type::Replay;
        break;
    case GameEngine::Transition::Quit:
        result = Command::Type::Quit;
        break;
    default:
        return false;
        break;
    }
    return true;
}
