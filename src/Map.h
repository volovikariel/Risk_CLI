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
    bool operator==(const Continent& other);
    friend std::ostream& operator<<(std::ostream& out, const Continent& source);

    int ID;
    std::string name;
    std::string color;
    int bonus;

    std::vector<Territory*> territories;
};

class Territory
{
public:

    Territory();
    Territory(int ID, const std::string& name, Continent* const continent, int x, int y);
    ~Territory();

    Territory(const Territory& other);
    bool operator==(const Territory& other);
    friend std::ostream& operator << (std::ostream &out, const Territory& source);

    int ID;
    std::string name;
    Continent* continent;
    int x;
    int y;
    std::vector<Territory*> neighbors;

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
        NotConnectedGraph,
        NotConnectedContinent,
        TerritoryNotInAContinent,
        TerritoryInMultipleContinents
    };

    friend std::ostream& operator << (std::ostream& out, const FormatError source);

    Map();
    ~Map();

    Map(const Map& other);
    bool operator==(const Map& other);
    friend std::ostream& operator << (std::ostream &out, const Map& source);

    FormatError validate() const;

    inline const std::vector<Continent*>& getContinents() const
    {
        return continents;
    }

    inline const std::vector<Territory*>& getTerritories() const
    {
        return territories;
    }

    std::string name;

private:

    std::vector<Continent*> continents;
    std::vector<Territory*> territories;
};

class MapLoader
{
public:

    MapLoader();
    ~MapLoader();

    MapLoader(const MapLoader& other);
    bool operator==(const MapLoader& other);
    friend std::ostream& operator<<(std::ostream& out, const MapLoader& source);

    Map::FormatError load(const std::string& filepath, Map& destination) const;
};
