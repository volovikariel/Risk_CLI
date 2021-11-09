#include "LoggingObserver.h"

// Constructor for Subject which initializes an empty Observer* list
Subject::Subject():
    observers(new list<Observer*>)
{

}

// Destructor for the Subject
Subject::~Subject()
{
    // Delete the individual observer pointers
    for(Observer* observer: *observers) {
        delete observer;
    }
    // Delete the observers list pointer
    delete observers;
}

// Attaches an observer to the subject
void Subject::attach(Observer* o)
{
    observers->push_back(o);
}

// Dettaches an observer to the subject
void Subject::detach(Observer* o)
{
    observers->remove(o);
}

// Notifies (calls update on) all the attached observers
void Subject::notify()
{
    for(Observer* observer: *observers) {
        observer->update((dynamic_cast<ILoggable*>(this)));
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
