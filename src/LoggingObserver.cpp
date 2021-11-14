#include "LoggingObserver.h"
#include <iostream>
#include <fstream>

// Constructor for Subject which initializes an empty Observer* list
Subject::Subject()
{
    list<Observer*> temp;
    observers = temp;
}

// Destructor for the Subject
Subject::~Subject()
{
    
}

// Attaches an observer to the subject
void Subject::attach(Observer& o)
{
    observers.push_back(&o);
}

// Dettaches an observer to the subject
void Subject::detach(Observer& o)
{
    observers.remove(&o);
}

// Notifies (calls update on) all the attached observers
void Subject::notify()
{
    for(Observer* observer: observers) {
        observer->update(*this);
    }
}

// Constructor for Observer
Observer::Observer()
{

}
// Destructor for Observer
Observer::~Observer()
{

}


// LogObserver constructor
LogObserver::LogObserver()
{
    // Clear the file - the game.txt file only contains the record of a single game at a time
    ofstream ofs;
    ofs.open("game.txt", std::ios_base::out | std::ios_base::trunc);
    ofs.close();
}

// LogObserver destructor
LogObserver::~LogObserver()
{
    
}

void LogObserver::update(Subject& log)
{
    ofstream ofs;
    ofs.open("game.txt", std::ios_base::app);
    ofs << dynamic_cast<ILoggable*>(&log)->stringToLog();
    ofs.close();
}