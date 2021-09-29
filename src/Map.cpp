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
    territories = other.territories;
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
        bonus == other.bonus &&
        territories == other.territories;
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
    neighbors = other.neighbors;
    player = other.player;
    armies = other.armies;
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
        neighbors == other.neighbors &&
        player == other.player &&
        armies == other.armies;
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

Map::Map(const Map& other)
{
    // TODO
}

Map::~Map()
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

void DFS(const std::vector<Territory*>& territories, const Territory* const node, std::vector<bool>& visited)
{
    int territoryIndex = node->ID - 1;
    visited[territoryIndex] = true;

    for (const Territory* const neighbor : node->neighbors)
    {
        int neighborIndex = neighbor->ID - 1;

        // Modified DFS: only explores neighbors which are included in the territories argument
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
    return this == &other;
}

std::ostream& operator<<(std::ostream& out, const MapLoader& source)
{
    return out;
}

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

                int continentIndex = continentID - 1;

                // Check that the continent exists
                if (continentIndex < 0 || continentIndex >= destination.continents.size())
                {
                    return Map::FormatError::BadTerritoryFormat;
                }

                Continent* continent = destination.continents.at(continentIndex);
                Territory* territory = new Territory(ID, name, continent, x, y);

                continent->territories.push_back(territory);
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

                int territoryIndex = ID - 1;

                // Check that territory exists
                if (territoryIndex < 0 || territoryIndex >= destination.territories.size())
                {
                    return Map::FormatError::BadBorderFormat;
                }

                Territory* territory = destination.territories.at(territoryIndex);

                int neighborID;
                while (l >> neighborID)
                {
                    int neighborIndex = neighborID - 1;

                    // Check that neighbor exists
                    if (neighborIndex < 0 || neighborIndex >= destination.territories.size())
                    {
                        return Map::FormatError::BadBorderFormat;
                    }

                    Territory* neighbor = destination.territories.at(neighborIndex);
                    territory->neighbors.push_back(neighbor);
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
