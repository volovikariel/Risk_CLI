#pragma once
#include "LoggingObserver.h"
#include "GameEngine.h"

#include <fstream>
#include <string>
#include <vector>

// Represents a command (may include a string parameter)
class Command : public Subject
{
public:

    // A subset of GameEngine::Transition
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

    // Returns command type
    Type getType() const;

    // Returns argument (if required by the command type)
    const std::string& getArgument() const;

    // Set detailed effects of this command (after execution)
    void saveEffect(const std::string& description);
    //void saveEffect(std::string description);
    void saveEffect(const char* description);

private:

    // The type of command
    Type type;
    // Stores the argument (if required by the command type, otherwise an empty string)
    std::string argument;
    // Detailed effects (set externally after execution)
    std::string effect;
};

// Reads command from console input
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

    // Ask the user to input the next command
    // Returns null if the input was not a well-formed command
    Command* getCommand();

    // Checks if the command is valid given the game engine's current state
    bool validate(Command& command);

protected:

    // Prompts the user to input a well-formed command
    // Returns null if the input was not a well-formed command
    virtual Command* readCommand();

    // Saves a command to the internal list
    void saveCommand(Command& command);

    // A list of commands obtained from console input
    std::vector<Command*> commands;
    // A pointer to the game state
    GameEngine* gameEngine;
};

class FileLineReader
{
public:

    // Default constructor
    FileLineReader();
    // Parametrized constructor
    FileLineReader(const std::string& filepath);
    // Copy constructor
    FileLineReader(const FileLineReader& other);
    // Destructor
    ~FileLineReader();

    // Assignment operator
    FileLineReader& operator = (const FileLineReader& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const FileLineReader& source);

    // Reads the next line of the file
    std::string readLineFromFile();

    // Checks the good bit status on the input file
    bool good() const;

private:

    // Input file path
    std::string filepath;
    // Input file stream
    std::ifstream filestream;
};

// Adapter (Target is CommandProcessor, Adaptee is FileLineReader)
class FileCommandProcessorAdapter : public CommandProcessor
{
public:

    // Default constructor
    FileCommandProcessorAdapter();
    // Parametrized constructor
    FileCommandProcessorAdapter(GameEngine& gameEngine, FileLineReader& fileLineReader);
    // Copy constructor
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other);
    // Destructor
    ~FileCommandProcessorAdapter();

    // Assignment operator
    FileCommandProcessorAdapter& operator = (const FileCommandProcessorAdapter& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const FileCommandProcessorAdapter& source);

    // Checks the good bit status on the input file
    bool good() const;

private:

    // Reads the next line of the file and returns the corresponding command
    // Returns null if the line was not a well-formed command
    Command* readCommand() override;

    // Adaptee
    FileLineReader* fileLineReader;
};

// Helper functions for commands
namespace CommandProcessingUtils
{
    // Command::Type is a subset of GameEngine::Transition
    // The following functions convert between them
    GameEngine::Transition commandToTransition(Command::Type type);
    bool transitionToCommand(GameEngine::Transition transition, Command::Type& result);
};
