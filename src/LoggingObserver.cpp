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
    return *this;
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

// Copy constructor for Observer
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

// Assignment operator
Observer& Observer::operator = (const Observer& other)
{
    return *this;
}

// Destructor for Observer
Observer::~Observer()
{

}

/* --- LogObserver --- */

// LogObserver constructor
LogObserver::LogObserver():
    filepath("gamelog.txt")
{
    filestream.open(filepath);
}

// LogObserver parametrized constructor
LogObserver::LogObserver(const std::string& filepath):
    filepath(filepath)
{
    filestream.open(filepath);
}

// Copy constructor for Observer
LogObserver::LogObserver(const LogObserver& other):
    filepath(other.filepath)
{
    filestream.open(filepath);
}

// Stream insertion operator
ostream& operator << (ostream& out, const LogObserver& source)
{
    // The LogObservor doesn't really have any properties we can print out, so we just print out that it's an LogObservor
    out << "LogObserver filepath: " << source.filepath;
    return out;
}

// Assignment operator
LogObserver& LogObserver::operator = (const LogObserver& other)
{
    // Close old opened file
    if (isOpen())
    {
        filestream.close();
    }

    filepath = other.filepath;
    filestream.open(filepath);

    return *this;
}

// LogObserver destructor
LogObserver::~LogObserver()
{
    // filestream closes file automatically in its destructor, no need to call explicitly
}


// LogObserver update function inherited from the Observer class and overriden
// log parameter must implement ILoggable 
void LogObserver::update(Subject& log)
{
    // The stringToLog function is only available for classes that inherit ILoggable, so we use dynamic_cast
    ILoggable* loggable = dynamic_cast<ILoggable*>(&log);

    // Write to file
    filestream << loggable->stringToLog() << endl;
    filestream.flush();
}

// Returns status of log file
bool LogObserver::isOpen() const
{
    return filestream.is_open();
}

/* --- ILoggable --- */

// ILoggable default constructor
ILoggable::ILoggable()
{

}

// Copy constructor for ILoggable
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

// Assignment operator
ILoggable& ILoggable::operator = (const ILoggable& other)
{
    return *this;
}
