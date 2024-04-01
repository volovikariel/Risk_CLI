#include "CommandProcessing.h"
#include "GameEngine.h"
#include "LoggingObserver.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include "PlayerStrategies.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main()
{
    // Log to file
    LogObserver logObserver;

    // Game engine
    GameEngine gameEngine;

    // Create console input command processor
    CommandProcessor commandProcessor(gameEngine);

    // Attach our observer
    gameEngine.attach(logObserver);
    commandProcessor.attach(logObserver);

    // Keep reading commands from input
    while (true)
    {
        // Read command from input until we obtain a well-formed command
        Command* command = nullptr;
        while (command == nullptr || command->getType() == Command::Type::Invalid)
        {
            command = commandProcessor.getCommand();
        }

        // Check that the requested state transition is valid
        if (!commandProcessor.validate(*command))
        {
            // The requested state transition is invalid

            // Get game engine's current state
            GameEngine::State state = gameEngine.getState();

            // Resolve command to transition mapping
            GameEngine::Transition requestedTransition = CommandProcessingUtils::commandToTransition(command->getType());

            // Set detailed explanation for command status
            std::ostringstream stream;
            stream << "The current state is " << state << " which has no transition " << requestedTransition;
            command->saveEffect(stream.str());

            // Print command
            std::cout << std::endl;
            std::cout << "Invalid command in the current game engine state:" << std::endl;
            std::cout << *command << std::endl;
        }
        else
        {
            // The requested state transition is valid

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
            std::cout << *command << std::endl;

            if (success && command->getType() == Command::Type::GameStart) {
                const std::vector<Player*> players = gameEngine.getPlayers();
                std::vector<PlayerStrategy*> strategies;

                // Assign strategies
                for (Player* player : players) {
                    PlayerStrategy* strategy = new HumanPlayerStrategy(*player);
                    strategies.push_back(strategy);
                    player->setPlayerStrategy(*strategy);
                }
                gameEngine.mainGameLoop();

                for (PlayerStrategy* strategy : strategies) {
                    delete strategy;
                }
            }
        }
    }
}
