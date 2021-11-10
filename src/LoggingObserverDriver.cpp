#include "GameEngine.h"
#include "LoggingObserver.h"
#include "CommandProcessing.h"
#include "Orders.h"

class CommandProcessorTest : public CommandProcessor
{
    public:
        void saveCommandTest(Command* command) {
            this->saveCommand(*command);
        }
};

int main()
{
    // The Observer we'll be attaching
    LogObserver lo;

    // The Subjects we'll be observing
    Command c;
    CommandProcessorTest cp;
    Order o;
    OrdersList ol;
    GameEngine ge;

    // Attaching the Observer to each Subject
    c.attach(lo);
    cp.attach(lo);
    o.attach(lo);
    ol.attach(lo);
    ge.attach(lo);

    // Calling CommandProcessor's "saveCommand" method to test the Observer
    cp.saveCommandTest(new Command(Command::Type::GameStart));
    // Calling Order's "execute" method to test the Observer
    o.execute();
    // Calling Command's "saveEffect" method to test the Observer
    c.saveEffect("Command's effect");
    // Calling OrderList's "addOrder" method to test the Observer
    ol.addOrder(new Order());
    // Calling GameEngine's "transition" method to test the Observer
    ge.transitionState(GameEngine::State::Win);
}
