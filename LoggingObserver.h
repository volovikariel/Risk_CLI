#include <list>
#include <string>
using namespace std;

class Observer {
    public:
        virtual void update() = 0;
        Observer();
        virtual ~Observer();
};

class Subject {
    public:
        virtual void attach(Observer*);
        virtual void detach(Observer*);
        virtual void notify();
        Subject();
        virtual ~Subject();
    private:
        list<Observer*>* observers;
};

class ILoggable {
    public:
        virtual string stringToLog() = 0;
};

