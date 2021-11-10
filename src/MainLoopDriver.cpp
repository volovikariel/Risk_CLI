#include "GameEngine.h"
#include <iostream>


int main()
{
    std::cout << "======================================= Part 3  ======================================= " << std::endl;

    GameEngine* ge = new GameEngine();
    ge->mainGameLoop();

    std::cout << "======================================= Part 3 end =======================================\n" << std::endl;
}
