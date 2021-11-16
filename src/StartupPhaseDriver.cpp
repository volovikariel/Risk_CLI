#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Player.h"

#include <iostream>
#include <sstream>
#include <string>

int main()
{
    cout << "Welcome to the WarZone game.\nPlease follow the instructions." << endl;

    GameEngine gameEngine = GameEngine();

    // Create console input command processor
    CommandProcessor commandProcessor(gameEngine);

    LogObserver log;
    commandProcessor.attach(log);

    // Keep reading commands from input
    while (true)
    {
        // Read command from input until we obtain a well-formed command
        Command* command = nullptr;
        while (command == nullptr || command->getType() == Command::Type::Invalid)
        {
            command = commandProcessor.getCommand();
        }

        if (commandProcessor.validate(*command))
        {
            // Resolve command to transition mapping
            GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command->getType());

            // Execute command (if succesful, will also apply the state transition)
            // Details are saved to the command's effect string
            bool success = gameEngine.executeCommand(*command);

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
            std::cout << (*command) << std::endl;

            // Show players' state at startup
            if (success && command->getType() == Command::Type::GameStart)
            {
                // Show random order
                cout << "Random order of players:" << endl;
                for (const Player* player : gameEngine.getPlayers())
                {
                    cout << "* " << player->getPlayerName() << endl;
                }

                // Showing off what the players have
                cout << endl;
                for (const Player* player : gameEngine.getPlayers())
                {
                    cout << "Player " << player->getPlayerName() << ":" << endl;
                    cout << *player << endl;
                }
            }
        }
    }
}
