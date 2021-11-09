#pragma once
#include <list>
#include <string>
using namespace std;

class ILoggable {
    public:
        virtual string stringToLog() = 0;
};

class Observer {
    public:
        virtual void update(ILoggable* log) = 0;
        Observer();
        virtual ~Observer();
};

class Subject {
    public:
        void attach(Observer* observer);
        void detach(Observer* observer);
        void notify();
        Subject();
        ~Subject();
    private:
        list<Observer*>* observers;
};

