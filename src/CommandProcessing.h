#pragma once
#include "GameEngine.h"

#include <string>
#include <vector>

class Command
{
public:

    enum class Type
    {
        LoadMap,
        ValidateMap,
        AddPlayer,
        GameStart,
        Replay,
        Quit,
        NumTypes
    };

    // Stream output operator of Type
    friend std::ostream& operator << (std::ostream& out, const Type source);

    // Default constructor
    Command();
    // Parametrized constructor
    Command(Type type);
    // Parametrized constructor
    Command(Type type, std::string& argument);
    // Copy constructor
    Command(const Command& other);
    // Destructor
    ~Command();

    // Assignment operator
    Command& operator = (const Command& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const Command& source);

    Type getType() const;

    void saveEffect(std::string& description);

private:

    Type type;
    std::string argument;
    std::string effect;
};

class CommandProcessor
{
public:

    // Default constructor
    CommandProcessor();
    // Parametrized constructor
    CommandProcessor(GameEngine& gameEngine);
    // Copy constructor
    CommandProcessor(const CommandProcessor& other);
    // Destructor
    ~CommandProcessor();

    // Assignment operator
    CommandProcessor& operator = (const CommandProcessor& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const CommandProcessor& source);

    Command& getCommand();
    bool validate(Command& command);

private:

    Command& readCommand();
    void saveCommand(Command& command);

    std::vector<Command*> commands;
    GameEngine* gameEngine;
};

class FileCommandProcessorAdapter
{
public:

    // Default constructor
    FileCommandProcessorAdapter();
    // Parametrized constructor
    FileCommandProcessorAdapter(CommandProcessor& processor);
    // Copy constructor
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    // Destructor
    ~FileCommandProcessorAdapter();

    // Assignment operator
    FileCommandProcessorAdapter& operator = (const FileCommandProcessorAdapter& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const FileCommandProcessorAdapter& source);

    Command& getCommand();
    bool validate(Command& command);

private:

    CommandProcessor* commandProcessor;
    std::string filepath;
};

namespace CommandProcessingUtils
{
    GameEngine::Transition commandToTransition(Command::Type type);
    bool transitionToCommand(GameEngine::Transition transition, Command::Type& result);
};
