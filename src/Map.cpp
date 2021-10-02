#include "Map.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

/* --- Continent --- */

Continent::Continent():
    ID(0),
    name(""),
    color(""),
    bonus(0),
    territories(),
    territoryIDs()
{

}

Continent::Continent(int ID, const std::string& name, const std::string& color, int bonus):
    ID(ID),
    name(name),
    color(color),
    bonus(bonus),
    territories(),
    territoryIDs()
{

}

// Shallow copy of territory pointers
Continent::Continent(const Continent& other):
    ID(other.ID),
    name(other.name),
    color(other.color),
    bonus(other.bonus),
    territories(other.territories),
    territoryIDs(other.territoryIDs)
{

}

// Shallow copy of territory pointers
void Continent::operator = (const Continent& other)
{
    ID = other.ID;
    name = other.name;
    color = other.color;
    bonus = other.bonus;
    territories = other.territories;
    territoryIDs = other.territoryIDs;
}

Continent::~Continent()
{

}

std::ostream& operator << (std::ostream& out, const Continent& source)
{
    out << "[" << source.ID << "] " << source.name << " (" << source.bonus << ")";
    return out;
}


/* --- Territory --- */

Territory::Territory():
    ID(0),
    name(""),
    continent(nullptr),
    continentID(0),
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
    continentID(continent->ID),
    x(x),
    y(y),
    neighbors(),
    neighborIDs(),
    player(nullptr),
    armies(0)
{

}

// Shallow copy of continent and neighbor pointers
Territory::Territory(const Territory& other):
    ID(other.ID),
    name(other.name),
    continent(other.continent),
    continentID(other.continentID),
    x(other.x),
    y(other.y),
    neighbors(other.neighbors),
    neighborIDs(other.neighborIDs),
    player(other.player),
    armies(other.armies)
{
    
}

// Shallow copy of continent and neighbor pointers
void Territory::operator = (const Territory& other)
    
{
    ID = other.ID;
    name = other.name;
    continent = other.continent;
    continentID = other.continentID;
    x = other.x;
    y = other.y;
    neighbors = other.neighbors;
    neighborIDs = other.neighborIDs;
    player = other.player;
    armies = other.armies;
}

Territory::~Territory()
{

}

std::ostream& operator << (std::ostream &out, const Territory& source)
{
    out << "[" << source.ID << "] " << source.name << " (" << source.continent->name << ")";
    return out;
}


/* --- Map --- */

std::ostream& operator << (std::ostream& out, const Map::FormatError source)
{
    static const char* names[9] =
    {
        "None",
        "FileDoesNotExist",
        "BadContinentFormat",
        "BadTerritoryFormat",
        "BadBorderFormat",
        "NotConnectedGraph",
        "NotConnectedContinent",
        "TerritoryNotInAContinent",
        "TerritoryInMultipleContinents"
    };

    out << names[static_cast<size_t>(source)];
    return out;
}

Map::Map():
    name(""),
    continents(),
    territories()
{

}

// Makes a deep copy of the other map and its graph
Map::Map(const Map& other)
{
    deepCopy(other);
}

// Makes a deep copy of the other map and its graph
void Map::operator = (const Map& other)
{
    deepCopy(other);
}

// Full cleanup of continents and territories
Map::~Map()
{
    releaseAllocs();
}

std::ostream& operator << (std::ostream &out, const Map& source)
{
    out << source.name;
    return out;
}

Continent* Map::getContinentByID(int ID) const
{
    int index = ID - 1;

    if (index < 0 || index >= continents.size())
    {
        return nullptr;
    }
    else
    {
        return continents[index];
    }
}

Territory* Map::getTerritoryByID(int ID) const
{
    int index = ID - 1;

    if (index < 0 || index >= territories.size())
    {
        return nullptr;
    }
    else
    {
        return territories[index];
    }
}

// Modified DFS: only explores neighbors which are included in the territories argument
// (this is done to support validation case 2, where we must limit ourselves to territories
// of a specific continent).
void DFS(const std::vector<Territory*>& territories, const Territory* const node, std::vector<bool>& visited)
{
    int territoryIndex = node->ID - 1;
    visited[territoryIndex] = true;

    for (const Territory* const neighbor : node->neighbors)
    {
        int neighborIndex = neighbor->ID - 1;

        if (find(territories.begin(), territories.end(), neighbor) != territories.end())
        {
            if (!visited.at(neighborIndex))
            {
                DFS(territories, neighbor, visited);
            }
        }
    }
}

Map::FormatError Map::validate() const
{
    const size_t numTerritories = territories.size();

    // Validation 1: The map is a connected graph
    // Check that every territory can reach every other territory
    for (const Territory* const territory : territories)
    {
        std::vector<bool> visited(numTerritories);

        DFS(territories, territory, visited);

        if (find(visited.begin(), visited.end(), false) != visited.end())
        {
            return Map::FormatError::NotConnectedGraph;
        }
    }

    // Validation 2: Continents are connected subgraphs
    for (const Continent* const continent : continents)
    {
        // Check that every territory in this continent can reach every other
        // territory in this continent, using paths that do not leave the
        // continent.
        for (const Territory* const territory : continent->territories)
        {
            std::vector<bool> visited(numTerritories);

            DFS(continent->territories, territory, visited);

            // Check that every territory in this continent was reached
            for (const Territory* const territoryCheck : continent->territories)
            {
                int territoryCheckIndex = territoryCheck->ID - 1;

                if (visited.at(territoryCheckIndex) == false)
                {
                    return FormatError::NotConnectedContinent;
                }
            }
        }
    }

    // Validation 3: Each country belongs to one and only one continent
    for (const Territory* const territory : territories)
    {
        bool belongsToContinent = false;

        // Find continents that contain this territory
        for (const Continent* const continent : continents)
        {
            const std::vector<Territory*>& continentTerritories = continent->territories;

            // Check if this territory belongs to this continent
            if (find(continentTerritories.begin(), continentTerritories.end(), territory) != continentTerritories.end())
            {
                if (belongsToContinent) // Already belongs to another continent
                {
                    return Map::FormatError::TerritoryInMultipleContinents;
                }
                else
                {
                    belongsToContinent = true;
                }
            }
        }

        if (!belongsToContinent) // This territory belongs to no continent at all
        {
            return Map::FormatError::TerritoryNotInAContinent;
        }
    }

    return Map::FormatError::None;
}

void Map::releaseAllocs()
{
    for (const Continent* const continent : continents)
    {
        delete continent;
    }

    for (const Territory* const territory : territories)
    {
        delete territory;
    }
}

void Map::deepCopy(const Map& other)
{
    name = other.name;

    releaseAllocs();

    territories.clear();
    for (const Territory* const otherTerritory : other.territories)
    {
        Territory* newTerritory = new Territory(*otherTerritory);
        territories.push_back(newTerritory);
    }

    continents.clear();
    for (const Continent* const otherContinent : other.continents)
    {
        Continent* newContinent = new Continent(*otherContinent);
        continents.push_back(newContinent);

        // Re-assign territory pointers
        for (int i = 0; i < newContinent->territoryIDs.size(); ++i)
        {
            int territoryIndex = newContinent->territoryIDs[i] - 1;
            newContinent->territories[i] = territories.at(territoryIndex);
        }
    }

    for (Territory* newTerritory : territories)
    {
        // Re-assign continent pointer
        int continentIndex = newTerritory->continent->ID - 1;
        newTerritory->continent = continents[continentIndex];

        // Re-assign neighbor pointers
        for (int i = 0; i < newTerritory->neighborIDs.size(); ++i)
        {
            int territoryIndex = newTerritory->neighborIDs[i] - 1;
            newTerritory->neighbors[i] = territories.at(territoryIndex);
        }
    }
}


/* --- MapLoader --- */

MapLoader::MapLoader()
{

}

MapLoader::MapLoader(const MapLoader& other)
{

}

void MapLoader::operator = (const MapLoader& other)
{
    return;
}

MapLoader::~MapLoader()
{

}

std::ostream& operator << (std::ostream& out, const MapLoader& source)
{
    out << &source;
    return out;
}

// Tiny state machine for MapLoader::load
enum class ParserState
{
    Heading,
    Continent,
    Territory,
    Border
};

Map::FormatError MapLoader::load(const std::string& filepath, Map& destination, int& errorLine) const
{
    errorLine = 0;

    // Extract filename from filepath
    size_t sepIdx = 0;
    size_t dotIdx = filepath.size();
    for (size_t i = 0; i < filepath.size(); ++i)
    {
        const char& currentChar = filepath.at(i);
        if (currentChar == '/' || currentChar == '\\' || currentChar == ':')
        {
            sepIdx = i + 1;
        }
        if (currentChar == '.')
        {
            dotIdx = i;
        }
    }
    destination.name = filepath.substr(sepIdx, dotIdx - sepIdx);

    std::ifstream inputFile(filepath);
    std::string line;

    // Check that the file exists
    if (!inputFile.good())
    {
        return Map::FormatError::FileDoesNotExist;
    }

    ParserState state = ParserState::Heading;
    int continentID = 1;

    // Read line by line
    while (std::getline(inputFile, line))
    {
        errorLine++;

        // Skipping empty line
        if (line.length() <= 0)
        {
            continue;
        }
        else
        {
            std::istringstream l = std::istringstream(line);

            std::string firstWord;
            l >> firstWord;

            // Skipping comments and empty lines
            if (firstWord == ";" || firstWord.size() <= 0 || firstWord.front() == ';')
            {
                continue;
            }

            // Exit current state and prepare to parse a heading
            if (firstWord.front() == '[')
            {
                state = ParserState::Heading;
            }

            if (state == ParserState::Heading)
            {
                if (firstWord == "[continents]")
                {
                    state = ParserState::Continent;
                }
                else if (firstWord == "[countries]")
                {
                    state = ParserState::Territory;
                }
                else if (firstWord == "[borders]")
                {
                    state = ParserState::Border;
                }
            }
            else if (state == ParserState::Continent)
            {
                std::string name = firstWord;
                int bonus;
                std::string color;

                l >> bonus >> color;

                // Check for format errors
                if (l.fail())
                {
                    return Map::FormatError::BadContinentFormat;
                }

                Continent* continent = new Continent(continentID++, name, color, bonus);
                destination.continents.push_back(continent);
            }
            else if (state == ParserState::Territory)
            {
                int ID = -1;
                try
                {
                    ID = std::stoi(firstWord);
                }
                catch (const std::invalid_argument&)
                {
                    return Map::FormatError::BadTerritoryFormat;
                }
                catch (const std::out_of_range&)
                {
                    return Map::FormatError::BadTerritoryFormat;
                }

                std::string name;
                int continentID;
                int x;
                int y;

                l >> name >> continentID >> x >> y;

                // Check for format errors
                if (l.fail())
                {
                    return Map::FormatError::BadTerritoryFormat;
                }

                Continent* continent = destination.getContinentByID(continentID);

                // Check that the continent exists
                if (continent == nullptr)
                {
                    return Map::FormatError::BadTerritoryFormat;
                }

                Territory* territory = new Territory(ID, name, continent, x, y);

                continent->territories.push_back(territory);
                continent->territoryIDs.push_back(territory->ID);
                destination.territories.push_back(territory);

                // Ensure the territories are in perfect sequential order
                if (destination.territories.size() != ID)
                {
                    return Map::FormatError::BadTerritoryFormat;
                }
            }
            else if (state == ParserState::Border)
            {
                int ID = -1;
                try
                {
                    ID = std::stoi(firstWord);
                }
                catch (const std::invalid_argument&)
                {
                    return Map::FormatError::BadBorderFormat;
                }
                catch (const std::out_of_range&)
                {
                    return Map::FormatError::BadBorderFormat;
                }

                Territory* territory = destination.getTerritoryByID(ID);

                // Check that territory exists
                if (territory == nullptr)
                {
                    return Map::FormatError::BadBorderFormat;
                }

                int neighborID;
                while (l >> neighborID)
                {
                    // Self-loop
                    if (neighborID == ID)
                    {
                        return Map::FormatError::BadBorderFormat;
                    }

                    Territory* neighbor = destination.getTerritoryByID(neighborID);

                    // Check that neighbor exists
                    if (neighbor == nullptr)
                    {
                        return Map::FormatError::BadBorderFormat;
                    }

                    territory->neighbors.push_back(neighbor);
                    territory->neighborIDs.push_back(neighborID);
                }

                // Did we miss any borders because of a bad intermediate value?
                if (!l.eof())
                {
                    return Map::FormatError::BadBorderFormat;
                }
            }
        }
    }

    // Past this point there will only be graph errors
    // which aren't related to a specific line.
    errorLine = -1;

    inputFile.close();
    return destination.validate();
}
