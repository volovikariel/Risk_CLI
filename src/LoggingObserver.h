#pragma once
#include <list>
#include <string>
using namespace std;

// Forward declaration so that Observer can use Subject
class Subject;

class ILoggable {
    public:
        virtual string stringToLog() = 0;
};

class Observer {
    public:
        virtual void update(Subject& subject) = 0;
        Observer();
        virtual ~Observer();
};

class Subject {
    public:
        virtual void attach(Observer& observer);
        virtual void detach(Observer& observer);
        virtual void notify();
        Subject();
        ~Subject();
    private:
        list<Observer*> observers;
};


class LogObserver : public Observer {
    public:
        LogObserver();
        ~LogObserver();
        void update(Subject& subject);
};
