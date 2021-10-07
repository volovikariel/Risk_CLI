#include "GameEngine.h"
#include <iostream>


void GameEngine::GameEngineStart(){//the function that will start the game
    cout<<"You are now in the " << state<< " state of the Game menu."<< endl;
        cout << "Please enter the following command to start the game : loadmap --> "<<endl;
        getline(cin, UserInput);
        cout<<""<<endl;
        if (UserInput == "loadmap") {
            cout <<"Transitioning to the next state --> Map Loaded..." << endl;
            cout<<""<<endl;
            setState("Map Loaded");
            MapLoaded();

        } else {
            cout <<"You have not entered the right command."<< endl;
            cout<<""<<endl;
            GameEngineStart();
        }
    }

GameEngine::GameEngine() {//constructor called when object is created. will set the state to start when game begins
    state= "Start";
}

GameEngine::~GameEngine() {//destructor

}

void GameEngine::MapLoaded() {//will load the map chosen. 2 choices of command available
    cout << "You are now in the " << state
         << " state of the Game menu." << endl;

        cout << "Please enter the following command :  --> "<<endl;
        cout<<""<<endl;
        cout<<"loadmap\n";
        cout<<"or \n";
        cout<<"validatemap\n";
        getline(cin, UserInput);
        cout<<""<<endl;
        if (UserInput == "validatemap") {
            cout << "Transitioning to the next state --> Map Validated..." << endl;
            cout<<""<<endl;
            setState("Map Validated");
            MapValidated();
        } else if(UserInput =="loadmap") {// reloading the same state we are in.
            MapLoaded();
        }else {
                cout << "You have not entered the right command." << endl;
                cout<<""<<endl;
                MapLoaded();
            }
        }


void GameEngine::MapValidated() {//will validate the chosen map.
    cout << "You are now in the " << state
         << " state of the Game menu." << endl;

        cout << "Please enter the following command : addplayer --> "<<endl;
        getline(cin, UserInput);
        cout<<""<<endl;
        if (UserInput == "addplayer") {
            cout << "Transitioning to the next state --> Players Added..." << endl;
            cout<<""<<endl;
            setState("Players Added");
            PlayersAdded();
        } else {
            cout << "You have not entered the right command." << endl;
            cout<<""<<endl;
            MapValidated();
        }

    }


void GameEngine::PlayersAdded() {//will add players as need and then assign them a country. 2 choices of command
    cout << "You are now in the " << state
         << " state of the Game menu." << endl;

        cout << "Please enter the following command :  --> "<<endl;
        cout<<""<<endl;
        cout<<"addplayer\n";
        cout<<"or \n";
        cout<<"assigncountries\n";
        getline(cin, UserInput);
        cout<<""<<endl;
        if (UserInput == "assigncountries") {
            cout << "Transitioning to the next state --> Assign Reinforcement..." << endl;
            cout<<""<<endl;
            setState("Assign Reinforcement");
            AssignReinforcements();
        } else if(UserInput =="addplayer") {
            PlayersAdded();//Re-loading the same state we are in
        }else {
            cout << "You have not entered the right command." << endl;
            cout<<""<<endl;
            PlayersAdded();
        }
    }


void GameEngine::AssignReinforcements() {// will assign reinforcements as needed
    cout << "You are now in the " << state
         << " state of the Game menu." << endl;

        cout << "Please enter the following command : issueorder --> "<<endl;
        getline(cin, UserInput);
        cout<<""<<endl;
        if (UserInput == "issueorder") {
            cout << "Transitioning to the next state --> Issue Orders..." << endl;
            cout<<""<<endl;
            setState("Issue Orders");
            IssueOrders();
        } else {
            cout << "You have not entered the right command." << endl;
            cout<<""<<endl;
            AssignReinforcements();
        }

    }

void GameEngine::IssueOrders() {//issue orders. 2 choices of command available
    cout << "You are now in the " << state
         << " state of the Game menu." << endl;

        cout << "Please enter the following command :  --> "<<endl;
        cout<<""<<endl;
        cout<<"issueorder\n";
        cout<<"or \n";
        cout<<"endissueorders\n";
        getline(cin, UserInput);
        cout<<""<<endl;
        if (UserInput == "endissueorders") {
            cout << "Transitioning to the next state --> Execute Orders..." << endl;
            cout<<""<<endl;
            setState("Execute Orders");
            ExecuteOrders();
        } else if(UserInput =="issueorder") {
            IssueOrders(); //re-laoding the state we are in
        }else {
            cout << "You have not entered the right command." << endl;
            cout<<""<<endl;
            IssueOrders();
        }
    }


void GameEngine::ExecuteOrders() {// will execute the orders given. 3 choices of command available
    cout << "You are now in the " << state
         << " state of the Game menu." << endl;

        cout << "Please enter the following command :  --> "<<endl;
        cout<<""<<endl;
        cout<<"execorder\n";
        cout<<"or \n";
        cout<<"endexecorders\n";
        cout<<"or \n";
        cout<<"win\n";
        getline(cin, UserInput);
        cout<<""<<endl;
        if (UserInput == "win") {
            cout << "Transitioning to the next state --> Win..." << endl;
            cout<<""<<endl;
            setState("Win");
            Win();
        } else if(UserInput =="execorder") {
            ExecuteOrders();//re-loading the state we are in
        }else if (UserInput =="endexecorders") {
            cout << "Transitioning to the previous state --> Assign Reinforcement..." << endl;
            cout<<""<<endl;
            setState("Assign Reinforcement");
            AssignReinforcements();
        }else {
                cout << "You have not entered the right command." << endl;
                cout<<""<<endl;
                ExecuteOrders();
            }
        }


void GameEngine::Win() {// will help determine if you end the game or want to play another one.
    cout << "You are now in the " << state
         << " state of the Game menu." << endl;

        cout << "Please enter the following command : --> "<<endl;
        cout<<""<<endl;
        cout<<"play \n";
        cout<<"or \n";
        cout<<"end\n";
        getline(cin, UserInput);
        cout<<""<<endl;
        if (UserInput=="end") {
            cout << "" << endl;
            cout<<"The game has now ended. Thank you for playing";
        }
        else if(UserInput =="play"){
            cout << "You are starting a new game..." << endl;
            cout << "Transitioning to the first state --> Start..." << endl;
            cout<<""<<endl;
            setState("Start");
            GameEngineStart();
        }
        else{
            cout << "You have not entered the right command." << endl;
            cout<<""<<endl;
            Win();
        }
    }


void GameEngine::setState(const string &state) {//Will help set the next state
    GameEngine::state = state;
}

ostream &operator<<(ostream &os, const GameEngine &engine) { //stream insertion operator
    os << "state: " << engine.state;
    return os;
}

GameEngine::GameEngine(const GameEngine& copie) {//copy constructor
    this->state=copie.state;

}

GameEngine &GameEngine::operator=(const GameEngine &assign) {//assignment operator
    this->state=assign.state;

    return *this;
}













