#include "Map.h"

#include <iostream>

bool sameString(const char* lhs, const char* rhs)
{
    return strcmp(lhs, rhs) == 0;
}

void analyzeFile(const char* filepath)
{
    Map map;
    MapLoader loader;
    auto result = loader.load(filepath, map);

    std::cout << "Map: " << map.name << std::endl;
    std::cout << "Load error: " << result << std::endl;
    std::cout << std::endl;

    for (auto continent : map.getContinents())
    {
        std::cout << *continent << std::endl;
    }

    std::cout << std::endl;

    for (auto territory : map.getTerritories())
    {
        std::cout << *territory << std::endl;
    }

    std::cout << std::endl;

    for (auto territory : map.getTerritories())
    {
        std::cout << "Neighbors of: " << *territory << std::endl;

        for (auto neighbor : territory->neighbors)
        {
            std::cout << "    " << *neighbor << std::endl;
        }

        std::cout << std::endl;
    }
}

void loadFile(const char* filepath)
{
    Map map;
    MapLoader loader;
    auto result = loader.load(filepath, map);

    std::cout << "Map: " << map.name << std::endl;
    std::cout << "Load error: " << result << std::endl;
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    bool displayHelp = argc > 1 && (
        sameString(argv[1], "h") ||
        sameString(argv[1], "help") ||
        sameString(argv[1], "-h") ||
        sameString(argv[1], "-help") ||
        sameString(argv[1], "--h") ||
        sameString(argv[1], "--help") ||
        sameString(argv[1], "/h") ||
        sameString(argv[1], "/help"));

    if (argc <= 1 || displayHelp)
    {
        std::cout << "Usage: provide one or more filepaths as arguments." << std::endl;
        std::cout << "If only one filepath is provided: the file will be loaded and its load status, continent, and territory details will be displayed." << std::endl;
        std::cout << "If multiple filepaths are provided: each file will be loaded and only its load status will be displayed." << std::endl;
        std::cout << std::endl;
    }
    else if (argc == 2)
    {
        char* filepath = argv[1];
        analyzeFile(filepath);
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            char* filepath = argv[i];
            loadFile(filepath);
        }
    }
}
