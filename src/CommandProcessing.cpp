#include "CommandProcessing.h"

#include <iostream>
#include <sstream>

/* --- Command --- */

std::ostream& operator << (std::ostream& out, const Command::Type source)
{
    static const char* names[6] =
    {
        "LoadMap",
        "ValidateMap",
        "AddPlayer",
        "GameStart",
        "Replay",
        "Quit"
    };

    out << names[static_cast<size_t>(source)];
    return out;
}

Command::Command():
    type(),
    argument(),
    effect()
{

}

Command::Command(Type type):
    type(type),
    argument(),
    effect()
{

}

Command::Command(Type type, std::string& argument) :
    type(type),
    argument(argument),
    effect()
{

}

Command::Command(const Command& other):
    type(other.type),
    argument(other.argument),
    effect(other.effect)
{

}

Command::~Command()
{

}

Command& Command::operator = (const Command& other)
{
    this->type = other.type;
    this->argument = other.argument;
    this->effect = other.effect;
    return *this;
}

std::ostream& operator << (std::ostream& out, const Command& source)
{
    out << "Command [" << source.type << "]";
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

void Command::saveEffect(std::string& description)
{
    effect = description;
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

Command& CommandProcessor::getCommand()
{
    return readCommand();
}

bool CommandProcessor::validate(Command& command)
{
    GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command.getType());
    return gameEngine->getStateInfo().canDoTransition(requestedTransition);
}

Command& CommandProcessor::readCommand()
{
    Command* requestedCommand = nullptr;
    bool fullCommandProvided = false;

    while (!fullCommandProvided)
    {
        std::cout << std::endl;
        std::cout << "Input one of the following commands:" << std::endl;

        auto& possibleTransitions = gameEngine->getStateInfo().getTransitions();
        size_t numCommands = static_cast<size_t>(Command::Type::NumTypes);
        for (size_t i = 0; i < numCommands; ++i)
        {
            Command::Type commandType = static_cast<Command::Type>(i);
            GameEngine::Transition transition = CommandProcessingUtils::commandToTransition(commandType);

            std::ostringstream stream;
            stream << transition;
            std::string transitionName = StringUtils::ToLowerCase(stream.str());

            bool isValidContext = std::find(possibleTransitions.begin(), possibleTransitions.end(), transition) != possibleTransitions.end();

            if (isValidContext)
            {
                std::cout << "[";
            }
            std::cout << transitionName;

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

        std::string line;
        std::getline(std::cin, line);
        if (line.length() == 0)
        {
            continue;
        }

        std::istringstream lineStream = std::istringstream(line);

        std::string command;
        std::string argument;

        lineStream >> command;
        if (lineStream.fail())
        {
            continue;
        }

        GameEngine::Transition transition;
        if (!StringUtils::ToGameEngineTransition(command, transition))
        {
            std::cout << "Unknown command" << std::endl;
            continue;
        }
        Command::Type commandType;
        if (!CommandProcessingUtils::transitionToCommand(transition, commandType))
        {
            std::cout << "Unknown command" << std::endl;
            continue;
        }

        if (transition == GameEngine::Transition::LoadMap || transition == GameEngine::Transition::AddPlayer)
        {
            lineStream >> argument;
            if (lineStream.fail())
            {
                std::cout << "Didn't receive the expected argument" << std::endl;
                continue;
            }

            requestedCommand = new Command(commandType, argument);
            fullCommandProvided = true;
        }
        else
        {
            requestedCommand = new Command(commandType);
            fullCommandProvided = true;
        }
    }
    
    saveCommand(*requestedCommand);
    return *requestedCommand;
}

void CommandProcessor::saveCommand(Command& command)
{
    commands.push_back(&command);
}




/* --- FileCommandProcessorAdapter --- */

FileCommandProcessorAdapter::FileCommandProcessorAdapter():
    commandProcessor(),
    filepath()
{

}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(CommandProcessor& processor):
    commandProcessor(&processor),
    filepath()
{

}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other):
    commandProcessor(other.commandProcessor),
    filepath(other.filepath)
{

}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter()
{

}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator = (const FileCommandProcessorAdapter& other)
{
    this->commandProcessor = other.commandProcessor;
    this->filepath = other.filepath;
    return *this;
}

std::ostream& operator << (std::ostream& out, const FileCommandProcessorAdapter& source)
{
    out << "Reading file: " << source.filepath;
    if (source.commandProcessor != nullptr)
    {
        out << std::endl;
        out << source.commandProcessor;
    }
    return out;
}

Command& FileCommandProcessorAdapter::getCommand()
{
    // TODO
    return Command();
}

bool FileCommandProcessorAdapter::validate(Command& command)
{
    // TODO
    return true;
}




/* --- CommandProcessingUtils --- */

GameEngine::Transition CommandProcessingUtils::commandToTransition(Command::Type type)
{
    switch (type)
    {
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
    }
}

bool CommandProcessingUtils::transitionToCommand(GameEngine::Transition transition, Command::Type& result)
{
    switch (transition)
    {
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