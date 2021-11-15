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

// Copy constructor for Subject
Subject::Subject(const Subject& other):
    observers(other.observers)
{
    
}

// Stream insertion operator
ostream& operator << (ostream& out, const Subject& source)
{
    // Printing out the size of the observers list
    out << "#Observers: " << source.observers.size();
    return out;
}

// Assignment operator
Subject& Subject::operator = (const Subject& other)
{
    // Shallow copy the Observer pointers
    this->observers = other.observers;
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

// Copy constructor for Observer (unused)
Observer::Observer(const Observer& other)
{

}

// Stream insertion operator
ostream& operator << (ostream& out, const Observer& source)
{
    // The Observor doesn't really have any properties we can print out, so we just print out that it's an Observor
    out << "Observer";
    return out;
}

// Assignment operator (unused)
Observer& Observer::operator = (const Observer& other)
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

// Copy constructor for Observer (unused)
LogObserver::LogObserver(const LogObserver& other)
{

}

// Stream insertion operator
ostream& operator << (ostream& out, const LogObserver& source)
{
    // The LogObservor doesn't really have any properties we can print out, so we just print out that it's an LogObservor
    out << "LogObserver";
    return out;
}

// Assignment operator (unused)
LogObserver& LogObserver::operator = (const LogObserver& other)
{

}

// LogObserver destructor
LogObserver::~LogObserver()
{
    
}


// LogObserver update function inherited from the Observer class and overriden
// Outputs to a "gamelog.txt" file logs of what is going on
void LogObserver::update(Subject& log)
{
    ofstream ofs;
    // We're appending to the file, so we specify "app"
    ofs.open("gamelog.txt", std::ios_base::app);
    // The stringToLog function is only available for classes that inherit ILoggable, so we use dynamic_cast to make the compiler happy
    ofs << dynamic_cast<ILoggable*>(&log)->stringToLog();
    // Close the file stream
    ofs.close();
}

/* --- ILoggable --- */

// ILoggable default constructor
ILoggable::ILoggable()
{

}

// Copy constructor for ILoggable (unused)
ILoggable::ILoggable(const ILoggable& other)
{

}

// Stream insertion operator
ostream& operator << (ostream& out, const ILoggable& source)
{
    // ILoggable doesn't really have any properties we can print out, so we just print out that it's an ILoggable
    out << "ILoggable";
    return out;
}

// Assignment operator (unused)
ILoggable& ILoggable::operator = (const ILoggable& other)
{

}