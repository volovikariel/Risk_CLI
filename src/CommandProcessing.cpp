#include "CommandProcessing.h"
#include "GameEngine.h"

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
    effect()
{

}

Command::Command(Type type):
    type(type),
    effect()
{

}

Command::Command(const Command& other):
    type(other.type),
    effect(other.effect)
{

}

Command::~Command()
{

}

Command& Command::operator = (const Command& other)
{
    this->type = other.type;
    this->effect = other.effect;
    return *this;
}

std::ostream& operator << (std::ostream& out, const Command& source)
{
    out << "Command [" << source.type << "] " << source.effect;
    return out;
}

Command::Type Command::getType() const
{
    return type;
}

void Command::safeEffect(std::string& description)
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
    // TODO
    return Command();
}

bool CommandProcessor::validate()
{
    // TODO
    return true;
}

void CommandProcessor::readCommand()
{
    // TODO
}

void CommandProcessor::saveCommand(Command& command)
{
    // TODO
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

bool FileCommandProcessorAdapter::validate()
{
    // TODO
    return true;
}
