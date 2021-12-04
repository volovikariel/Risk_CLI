#pragma once
#include <iostream>
#include <fstream>
#include <list>
#include <string>
using namespace std;

// Forward declaration so that Observer can use Subject
class Subject;

// ILoggable abstract class which requires the inheriting class to implement a stringToLog function
class ILoggable {
    public:
        // Default constructor
        ILoggable();
        // Stream output operator of ILoggable
        friend std::ostream& operator << (std::ostream& out, const ILoggable& source);
        // String to log function whose implementation specifies what is logged to the "gamelog.txt" file
        virtual string stringToLog() = 0;

    private:
        // Observer copy constructor
        ILoggable(const ILoggable& other);
        // Assignment operator of ILoggable
        ILoggable& operator = (const ILoggable& other);

};

class Observer {
    public:
        // Observer constructor
        Observer();
        // Observer destructor
        virtual ~Observer();
         // Stream output operator of Observer
        friend std::ostream& operator << (std::ostream& out, const Observer& source);
        // Update method which signifies an update to the state was received and we will act on it (be it updating a view or in this case, writing to a file)
        virtual void update(Subject& subject) = 0;

    private:
        // Observer copy constructor
        Observer(const Observer& other);
        // Assignment operator of Observer
        Observer& operator = (const Observer& other);
};

// Subject class which represents the Subject that will be observed
class Subject {
    public:
        // Subject constructor
        Subject();
        // Subject copy constructor
        Subject(const Subject& other);
        // Subject destructor
        ~Subject();
        // Assignment operatorof Subject
        Subject& operator = (const Subject& other);
        // Stream output operator of Subject
        friend std::ostream& operator << (std::ostream& out, const Subject& source);
        // Attach method which adds an Observer to "observers"
        virtual void attach(Observer& observer);
        // Detach method which removes an Observer from "observers"
        virtual void detach(Observer& observer);
        // Method which calls the "update" method for each Observer in "observers"
        virtual void notify();
       
    protected:
        list<Observer*> observers;
};

// LogObserver class which is a specific type of Observer which allows for logging
class LogObserver : public Observer {
    public:
        // LogObserver constructor
        LogObserver();
        // LogObserver parametrized constructor
        LogObserver(const std::string& filepath);
        // LogObserver destructor
        ~LogObserver();
        // Stream output operator of LogObserver
        friend std::ostream& operator << (std::ostream& out, const LogObserver& source);
        // Update method implementation inherited from Observer
        void update(Subject& subject);
        // Writes a line to the file
        void appendLine(const char* line);
        // Returns status of log file
        bool isOpen() const;

    private:
        // LogObserver copy constructor
        LogObserver(const LogObserver& other);
        // Assignment operator of LogObserver
        LogObserver& operator = (const LogObserver& other);

        string filepath;
        ofstream filestream;
};
