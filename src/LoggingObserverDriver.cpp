#include "GameEngine.h"
#include "LoggingObserver.h"
#include "CommandProcessing.h"
#include "Orders.h"

// Exposes protected function saveCommand for use by this driver
class CommandProcessorTest : public CommandProcessor
{
public:

    void saveCommandTest(Command* command)
    {
        saveCommand(*command);
    }
};

int main()
{
    // The Observer we'll be attaching
    LogObserver logObserver;

    // The Subjects we'll be observing
    Command command;
    CommandProcessorTest commandProcessor;
    Bomb* order = new Bomb();
    OrdersList ordersList;
    GameEngine gameEngine;

    // Attaching the Observer to each Subject
    command.attach(logObserver);
    commandProcessor.attach(logObserver);
    order->attach(logObserver);
    ordersList.attach(logObserver);
    gameEngine.attach(logObserver);

    // Calling CommandProcessor's "saveCommand" method to test the Observer
    commandProcessor.saveCommandTest(new Command(Command::Type::GameStart));
    // Calling Order's "execute" method to test the Observer
    order->execute();
    // Calling Command's "saveEffect" method to test the Observer
    command.saveEffect("Command's effect");
    // Calling OrderList's "addOrder" method to test the Observer
    ordersList.addOrder(order);
    // Calling GameEngine's "transition" method to test the Observer
    gameEngine.transitionState(GameEngine::State::MapLoaded);
    
    // Expected:
    /*
        CommandProcessor: ...
        Order: ...
        Command: ...
        OrderList: ...
        GameEngine: ...
    */
}
