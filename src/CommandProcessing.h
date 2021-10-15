#pragma once
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
        Quit
    };

private:

    void saveCommand();

    std::string effect;
};

class CommandProcessor
{
public:

    void getCommand();
    bool validate();

private:

    void readCommand();
    void saveCommand();

    std::vector<Command*> commands;
};
