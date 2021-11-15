#include "LoggingObserver.h"
#include <iostream>
#include <fstream>

/* --- Subject --- */

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

// Dettaches an observer from the subject
void Subject::detach(Observer& o)
{
    observers.remove(&o);
}

// Notifies (calls "update" on) all the attached observers
void Subject::notify()
{
    for(Observer* observer: observers) {
        observer->update(*this);
    }
}

/* --- Observer --- */

// Constructor for Observer
Observer::Observer()
{

}
// Destructor for Observer
Observer::~Observer()
{

}

/* --- LogObserver --- */

// LogObserver constructor
LogObserver::LogObserver()
{

}

// LogObserver destructor
LogObserver::~LogObserver()
{
    
}

// LogObserver update function inherited from the Observer class and overriden
// Outputs to a "game.txt" file logs of what is going on
void LogObserver::update(Subject& log)
{
    ofstream ofs;
    // We're appending to the file, so we specify "app"
    ofs.open("game.txt", std::ios_base::app);
    // The stringToLog function is only available for classes that inherit ILoggable, so we use dynamic_cast to make the compiler happy
    ofs << dynamic_cast<ILoggable*>(&log)->stringToLog();
    // Close the file stream
    ofs.close();
}