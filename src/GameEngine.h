#include <string>
#include <ostream>

using namespace std;

class GameEngine
{
public :

    GameEngine();//Default constructor

    ~GameEngine();//destructor

    void GameEngineStart();// to start the game

    void setState(const string &state);//will help set the state when transitioning

    string UserInput; //will store using input

    string state; //will hold the state

    friend ostream &operator<<(ostream &os, const GameEngine &engine); //stream insertion operator

    GameEngine (const GameEngine& copie); // copy constructor

    GameEngine& operator = (const GameEngine& assign);//assignment operator



private:
    void MapLoaded(); // function that will show if the map is  loaded
    void MapValidated(); // function that will show if the map loaded is valid
    void PlayersAdded(); // function that will add player or assign countries
    void AssignReinforcements(); // function that will determine what orders are issued
    void IssueOrders(); // function that will show if the orders have to end or more to be added
    void ExecuteOrders(); // function that will determine if there is a winner or more needs to be done
    void Win(); // function that will determine if the game will end or another will be played


};

