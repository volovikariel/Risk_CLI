#include <iostream>
#include <string>
#include <vector>

class Player;
class Territory;

class Continent
{
public:

    Continent();
    Continent(int ID, const std::string& name, const std::string& color, int bonus);
    ~Continent();

    Continent(const Continent& other);
    void operator = (const Continent& other);
    friend std::ostream& operator << (std::ostream& out, const Continent& source);

    int ID;
    std::string name;
    std::string color;
    int bonus;

    std::vector<Territory*> territories;
    std::vector<int> territoryIDs;
};

class Territory
{
public:

    Territory();
    Territory(int ID, const std::string& name, Continent* const continent, int x, int y);
    ~Territory();

    Territory(const Territory& other);
    void operator = (const Territory& other);
    friend std::ostream& operator << (std::ostream &out, const Territory& source);

    int ID;
    std::string name;
    Continent* continent;
    int continentID;
    int x;
    int y;
    std::vector<Territory*> neighbors;
    std::vector<int> neighborIDs;

    Player* player;
    int armies;
};

class Map
{
    friend class MapLoader;

public:

    enum class FormatError
    {
        None,
        FileDoesNotExist,
        BadContinentFormat,
        BadTerritoryFormat,
        BadBorderFormat,
        NotConnectedGraph,
        NotConnectedContinent,
        TerritoryNotInAContinent,
        TerritoryInMultipleContinents
    };

    friend std::ostream& operator << (std::ostream& out, const FormatError source);

    Map();
    ~Map();

    Map(const Map& other);
    void operator = (const Map& other);
    friend std::ostream& operator << (std::ostream &out, const Map& source);

    FormatError validate() const;

    std::string name;
    std::vector<Continent*> continents;
    std::vector<Territory*> territories;

private:

    void releaseAllocs();
    void deepCopy(const Map& other);
};

class MapLoader
{
public:

    MapLoader();
    ~MapLoader();

    MapLoader(const MapLoader& other);
    void operator = (const MapLoader& other);
    friend std::ostream& operator << (std::ostream& out, const MapLoader& source);

    Map::FormatError load(const std::string& filepath, Map& destination, int& errorLine) const;
};
