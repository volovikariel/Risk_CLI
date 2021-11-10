#include "LoggingObserver.h"

// Constructor for Subject which initializes an empty Observer* list
Subject::Subject()
{
    list<Observer*> temp;
    observers = temp;
}

// Destructor for the Subject
Subject::~Subject()
{
    // Delete the individual observer pointers
    for(Observer* observer: observers) {
        delete observer;
    }
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

}

// LogObserver destructor
LogObserver::~LogObserver()
{

}

void LogObserver::update(Subject& log)
{
    //TODO: Dynamic casting somehow
    // dynamic_cast<ILoggable>(log)
        //TODO: File IO here
        //log.stringToLog();
}

// TODO: Do we care about ILoggable constructor/destructor at all
