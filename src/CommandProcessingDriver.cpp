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
            // Keep reading commands from input
            while (true)
            {
                // Read command from input
                Command& command = commandProcessor.getCommand();

                // Check that the requested state transition is valid
                if (!commandProcessor.validate(command))
                {
                    // The requested state transition is invalid

                    // Get game engine's current state
                    GameEngine::State state = gameEngine.getState();

                    // Resolve command to transition mapping
                    GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command.getType());

                    // Set detailed explanation for command status
                    std::ostringstream stream;
                    stream << "The current state is " << state << " which has no transition " << requestedTransition;
                    command.saveEffect(stream.str());

                    // Print command
                    std::cout << std::endl;
                    std::cout << "Invalid command in the current game engine state:" << std::endl;
                    std::cout << command << std::endl;
                }
                else
                {
                    // The requested state transition is valid

                    // Resolve command to transition mapping
                    GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command.getType());

                    // Execute command (if succesful, will also apply the state transition)
                    // Details are saved to the command's effect string
                    bool success = gameEngine.executeCommand(command);

                    // Print execution result
                    std::cout << std::endl;
                    if (success)
                    {
                        std::cout << "Command execution succeeded:" << std::endl;
                    }
                    else
                    {
                        std::cout << "Command execution failed:" << std::endl;
                    }
                    std::cout << command << std::endl;
                }
            }
        }
        else if (firstArgument == "-file" && argc == 3)
        {
            // TODO implement FileCommandProcessorAdapter
        }
        else
        {
            printUsage = true;
        }
    }

    // Print user instructions
    if (printUsage)
    {
        std::cout << "Usage: -console to input commands through the console, or -file <filepath> to read commands from a file" << std::endl;
        std::cout << std::endl;
    }
}
