#include "Map.h"

#include <iostream>

int main(int argc, char *argv[])
{
    // TODO load any number of maps

    if (argc > 1)
    {
        char* filepath = argv[1];

        Map map;
        MapLoader loader;
        auto result = loader.load(filepath, map);
        std::cout << "Load error: " << result << std::endl << std::endl;

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
}
