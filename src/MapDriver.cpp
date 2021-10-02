#include "Map.h"

#include <iostream>
#include <string.h>

bool sameString(const char* lhs, const char* rhs)
{
    return strcmp(lhs, rhs) == 0;
}

void analyzeFile(const char* filepath)
{
    Map map;
    MapLoader loader;
    int errorLine;
    Map::FormatError result = loader.load(filepath, map, errorLine);

    std::cout << "Map: " << map.name << std::endl;
    if (errorLine > 0)
    {
        std::cout << "Load error: " << result << " on line " << errorLine << std::endl;
    }
    else
    {
        std::cout << "Load error: " << result << std::endl;
    }
    std::cout << std::endl;

    for (const Continent* const continent : map.continents)
    {
        std::cout << *continent << std::endl;
    }

    std::cout << std::endl;

    for (const Territory* const territory : map.territories)
    {
        std::cout << *territory << std::endl;
    }

    std::cout << std::endl;

    for (const Territory* const territory : map.territories)
    {
        std::cout << "Neighbors of: " << *territory << std::endl;

        for (const Territory* const neighbor : territory->neighbors)
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
    int errorLine;
    Map::FormatError result = loader.load(filepath, map, errorLine);

    std::cout << "Map: " << map.name << std::endl;
    if (errorLine > 0)
    {
        std::cout << "Load error: " << result << " on line " << errorLine << std::endl;
    }
    else
    {
        std::cout << "Load error: " << result << std::endl;
    }
    std::cout << std::endl;
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
        std::cout << "If only one filepath is provided:" << std::endl;
        std::cout << "    The file will be loaded and its load status, continent, and territory details will be displayed." << std::endl;
        std::cout << std::endl;
        std::cout << "If multiple filepaths are provided:" << std::endl;
        std::cout << "    Each file will be loaded and only its load status will be displayed." << std::endl;
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
