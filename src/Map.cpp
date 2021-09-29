#include "Map.h"

#include <fstream>
#include <iostream>
#include <sstream>

/* --- Continent --- */

Continent::Continent():
    ID(0),
    name(""),
    color(""),
    bonus(0),
    territories()
{

}

Continent::Continent(int ID, const std::string& name, const std::string& color, int bonus):
    ID(ID),
    name(name),
    color(color),
    bonus(bonus),
    territories()
{

}

Continent::Continent(const Continent& other)
{
    ID = other.ID;
    name = other.name;
    color = other.color;
    bonus = other.bonus;
    // TODO territories
}

Continent::~Continent()
{

}

bool Continent::operator==(const Continent& other)
{
    return
        ID == other.ID &&
        name == other.name &&
        color == other.color &&
        bonus == other.bonus;
    // TODO territories
}

std::ostream& operator<<(std::ostream& out, const Continent& source)
{
    out << "[" << source.ID << "] " << source.name << " (" << source.bonus << ")";
    return out;
}


/* --- Territory --- */

Territory::Territory():
    ID(0),
    name(""),
    continent(nullptr),
    x(0),
    y(0),
    neighbors(),
    player(nullptr),
    armies(0)
{

}

Territory::Territory(int ID, const std::string& name, Continent* const continent, int x, int y):
    ID(ID),
    name(name),
    continent(continent),
    x(x),
    y(y),
    neighbors(),
    player(nullptr),
    armies(0)
{

}

Territory::Territory(const Territory& other)
{
    ID = other.ID;
    name = other.name;
    continent = other.continent;
    x = other.x;
    y = other.y;
    player = other.player;
    armies = other.armies;
    // TODO neighbors
}

Territory::~Territory()
{

}

bool Territory::operator==(const Territory& other)
{
    return
        ID == other.ID &&
        name == other.name &&
        continent == other.continent &&
        x == other.x &&
        y == other.y &&
        player == other.player &&
        armies == other.armies;
    // TODO neighbors
}

std::ostream& operator << (std::ostream &out, const Territory& source)
{
    out << "[" << source.ID << "] " << source.name << " (" << source.continent->name << ")";
    return out;
}


/* --- Map --- */

Map::Map():
    name(""),
    continents(),
    territories()
{

}

Map::Map(const Map& other)
{
    // TODO
}

Map::~Map()
{
    // TODO
}

bool Map::operator==(const Map& other)
{
    // TODO
    return false;
}

std::ostream& operator << (std::ostream &out, const Map& source)
{
    out << source.name;
    return out;
}

void DFS(const std::vector<Territory*>& territories, int v, std::vector<bool>& visited)
{
    visited[v] = true;

    for (auto neighbor : territories.at(v)->neighbors)
    {
        int neighborIndex = neighbor->ID - 1;

        if (!visited.at(neighborIndex))
        {
            DFS(territories, neighborIndex, visited);
        }
    }
}

Map::FormatError Map::validate() const
{
    const size_t numTerritories = territories.size();

    for (int i = 0; i < numTerritories; ++i)
    {
        std::vector<bool> visited(numTerritories);

        DFS(territories, i, visited);

        if (find(visited.begin(), visited.end(), false) != visited.end())
        {
            return Map::FormatError::NotConnectedGraph;
        }
    }

    // TODO validations 2 and 3

    return Map::FormatError::None;
}


/* --- MapLoader --- */

MapLoader::MapLoader()
{

}

MapLoader::MapLoader(const MapLoader& other)
{

}

MapLoader::~MapLoader()
{

}

bool MapLoader::operator==(const MapLoader& other)
{
    return false;
}

std::ostream& operator<<(std::ostream& out, const MapLoader& source)
{
    return out;
}

enum ParserState
{
    HeadingState,
    ContinentState,
    TerritoryState,
    BorderState
};

Map::FormatError MapLoader::load(const std::string& filepath, Map& destination) const
{
    std::ifstream inputFile(filepath);
    std::string line;

    ParserState state = HeadingState;
    int continentID = 1;

    // Read line by line
    while (std::getline(inputFile, line))
    {
        if (line.length() <= 0) // Skipping empty line
        {
            continue;
        }
        else
        {
            std::istringstream l = std::istringstream(line);

            std::string firstWord;
            l >> firstWord;

            if (firstWord == ";") // Skipping comments
            {
                continue;
            }

            if (firstWord.at(0) == '[')
            {
                state = HeadingState;
            }

            if (state == HeadingState)
            {
                if (firstWord == "[continents]")
                {
                    state = ContinentState;
                }
                else if (firstWord == "[countries]")
                {
                    state = TerritoryState;
                }
                else if (firstWord == "[borders]")
                {
                    state = BorderState;
                }
            }
            else if (state == ContinentState)
            {
                std::string name = firstWord;
                int bonus;
                std::string color;

                l >> bonus >> color;

                Continent* continent = new Continent(continentID++, name, color, bonus);
                destination.continents.push_back(continent);
            }
            else if (state == TerritoryState)
            {
                int ID = std::stoi(firstWord);
                std::string name;
                int continentID;
                int x;
                int y;

                l >> name >> continentID >> x >> y;

                Continent* continent = destination.continents.at(continentID - 1);
                Territory* territory = new Territory(ID, name, continent, x, y);

                continent->territories.push_back(territory);
                destination.territories.push_back(territory);
            }
            else if (state == BorderState)
            {
                int ID = std::stoi(firstWord);
                int neighborID;

                Territory* territory = destination.territories.at(ID - 1);

                while (l >> neighborID)
                {
                    Territory* neighbor = destination.territories.at(neighborID - 1);
                    territory->neighbors.push_back(neighbor);
                }
            }
        }
    }

    inputFile.close();
    return destination.validate();
}
