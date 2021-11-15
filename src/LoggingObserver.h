#pragma once
#include <list>
#include <string>
using namespace std;

// Forward declaration so that Observer can use Subject
class Subject;

// ILoggable abstract class which requires the inheriting class to implement a stringToLog function
class ILoggable {
    public:
        // String to log function whose implementation specifies what is logged to the "game.txt" file
        virtual string stringToLog() = 0;
};

class Observer {
    public:
        // Observer constructor
        Observer();
        // Observer destructor
        virtual ~Observer();
        // Update method which signifies an update to the state was received and we will act on it (be it updating a view or in this case, writing to a file)
        virtual void update(Subject& subject) = 0;
};

// Subject class which represents the Subject that will be observed
class Subject {
    public:
        // Subject constructor
        Subject();
        // Subject destructor
        ~Subject();
        // Attach method which adds an Observer to "observers"
        virtual void attach(Observer& observer);
        // Detach method which removes an Observer from "observers"
        virtual void detach(Observer& observer);
        // Method which calls the "update" method for each Observer in "observers"
        virtual void notify();
    private:
        list<Observer*> observers;
};

// LogObserver class which is a specific type of Observer which allows for logging
class LogObserver : public Observer {
    public:
        // LogObserver constructor
        LogObserver();
        // LogObserver destructor
        ~LogObserver();
        // Update method implementation inherited from Observer
        void update(Subject& subject);
};
