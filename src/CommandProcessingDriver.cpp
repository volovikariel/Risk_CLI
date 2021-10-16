#include "CommandProcessing.h"
#include "GameEngine.h"

#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[])
{
    GameEngine gameEngine;
    CommandProcessor commandProcessor(gameEngine);

    bool printUsage = false;

    if (argc <= 1)
    {
        printUsage = true;
    }
    else
    {
        std::string firstArgument(argv[1]);

        if (firstArgument == "-console" && argc == 2)
        {
            while (true)
            {
                Command& command = commandProcessor.getCommand();

                if (!commandProcessor.validate(command))
                {
                    GameEngine::State state = gameEngine.getState();
                    GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command.getType());

                    std::ostringstream stream;
                    stream << "The current state is " << state << " which has no transition " << requestedTransition;
                    command.saveEffect(stream.str());

                    std::cout << std::endl;
                    std::cout << "Invalid command in the current game engine state:" << std::endl;
                    std::cout << command << std::endl;
                }
                else
                {
                    GameEngine::State oldState = gameEngine.getState();
                    GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command.getType());
                    gameEngine.transitionState(requestedTransition);
                    GameEngine::State newState = gameEngine.getState();

                    std::ostringstream stream;
                    stream << "Changed game state from " << oldState << " to " << newState;
                    command.saveEffect(stream.str());

                    std::cout << std::endl;
                    std::cout << "Executed command:" << std::endl;
                    std::cout << command << std::endl;
                }
            }
        }
        else if (firstArgument == "-file" && argc == 3)
        {

        }
        else
        {
            printUsage = true;
        }
    }

    if (printUsage)
    {
        std::cout << "Usage: -console to input commands through the console, or -file <filepath> to read commands from a file" << std::endl;
        std::cout << std::endl;
    }
}
