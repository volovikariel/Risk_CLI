#include <iostream>
#include <string>
#include <vector>

// Forward declarations to minimize dependencies
class Player;
class Territory;

// A connected sub-graph of territories
// Does not own any memory itself, the Map class manages allocations instead
class Continent
{
public:

    // Default constructor
    Continent();
    // Constructor with parameters
    Continent(int ID, const std::string& name, const std::string& color, int bonus);
    // Destructor
    ~Continent();

    // Copy constructor
    Continent(const Continent& other);
    // Assignment operator
    void operator = (const Continent& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const Continent& source);

    // Continent ID, as defined in the map file. Note: NOT 0-based
    int ID;
    // Continent name, as defined in the map file
    std::string name;
    // Continent color, as defined in the map file
    std::string color;
    // Continent army bonus, as defined in the map file
    int bonus;

    // Direct pointers to territories belonging to this continent
    std::vector<Territory*> territories;
    // IDs of the territories belonging to this continent
    std::vector<int> territoryIDs;
};

// Represents a territory
// Does not own any memory itself, the Map class manages allocations instead
class Territory
{
public:

    // Default constructor
    Territory();
    // Constructor with parameters
    Territory(int ID, const std::string& name, Continent* const continent, int x, int y);
    // Destructor
    ~Territory();

    // Copy constructor
    Territory(const Territory& other);
    // Assignment operator
    void operator = (const Territory& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream &out, const Territory& source);

    // Territory ID, as defined in the map file. Note: NOT 0 - based
    int ID;
    // Territory name, as defined in the map file
    std::string name;
    // Direct pointer to the continent that this territory belongs to
    Continent* continent;
    // ID of the continent that this territory belongs to, as defined in the map file
    int continentID;
    // Territory x position, as defined in the map file
    int x;
    // Territory y position, as defined in the map file
    int y;
    // Direct pointers to bordering territories
    std::vector<Territory*> neighbors;
    // IDs of bordering territories, as defined in the map file
    std::vector<int> neighborIDs;

    // Direct pointer to the player who owns this territory
    Player* player;
    // Number of the player's armies currently located inside this territory
    int armies;
};

// Contains a map, as defined by a map file.
// This class manages all allocations pertaining to continents and territories.
// The lifetimes of a Map instance's continents and territories are tied to the instance.
class Map
{
    // MapLoader should have full access to this class
    friend class MapLoader;

public:

    // Errors that can be encountered during map loading or validation
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

    // Stream output operator of FormatError
    friend std::ostream& operator << (std::ostream& out, const FormatError source);

    // Default constructor
    Map();
    // Destructor
    ~Map();

    // Copy constructor
    Map(const Map& other);
    // Assignment operator
    void operator = (const Map& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream &out, const Map& source);

    // The name of the map, determined from the filepath that MapLoader receives
    std::string name;
    // Continents in this map, ordered by ID
    // NOTE: IDs are 1-based, so vector index = ID - 1
    std::vector<Continent*> continents;
    // Territories in this map, ordered by ID
    // NOTE: IDs are 1-based, so vector index = ID - 1
    std::vector<Territory*> territories;

private:

    // Validates this map's graph connections. Called inside MapLoader::load.
    FormatError validate() const;

    // Releases all allocations pertaining to maps, continents, and territories
    void releaseAllocs();

    // Calls releaseAllocs, then makes a full duplicate of the other map
    // (including its territories and continents)
    void deepCopy(const Map& other);
};

// Loads map files from disk to populate Map instances
class MapLoader
{
public:

    // Default constructor
    MapLoader();
    // Destructor
    ~MapLoader();

    // Copy constuctor
    MapLoader(const MapLoader& other);
    // Assignment operator
    void operator = (const MapLoader& other);
    // Stream output operator
    friend std::ostream& operator << (std::ostream& out, const MapLoader& source);

    // Loads a map file, using the provided filepath, and filling a destination Map instance.
    // Reports any errors using the return value and errorLine for the specific line of the
    // file where it occured.
    Map::FormatError load(const std::string& filepath, Map& destination, int& errorLine) const;
};
