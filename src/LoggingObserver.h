#include <list>
#include <string>
using namespace std;

class ILoggable {
    public:
        virtual string stringToLog() = 0;
};

class Observer {
    public:
        virtual void update(ILoggable*) = 0;
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

