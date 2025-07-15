#include <SFML/Graphics.hpp>
#include <iostream>
#include "login.hpp"
#include "loading.hpp"

bool loadFont(sf::Font& font) {
    if (!font.loadFromFile("font/arial.ttf")) {
        std::cerr << "Failed to load font from font/arial.ttf\n";
        return false;
    }
    return true;
}

int main() {
    sf::Font font;
    if (!loadFont(font)) {
        std::cerr << "FATAL: Could not load font\n";
        return -1;
    }

    std::string loggedInUser;
    bool loggedIn = CS::runLoginWindow(font, loggedInUser);

    if (!loggedIn) {
        std::cout << "Login canceled or failed. Exiting...\n";
        return 0;
    }

    CS::showLoadingScreen(font);

    std::cout << "Welcome, " << loggedInUser << "! Starting game...\n";

    return 0;
}
