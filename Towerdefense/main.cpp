#include "login.h"
#include "Game.h"
#include <iostream>

int main() {
    if (login()) {
        // User logged in and pressed Play Game — start your main game here
        runGame(); // call your game loop function
    }
    else {
        std::cout << "Login failed or user quit." << std::endl;
        return 0;
    }
    return 0;
}