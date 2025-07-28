#include <SFML/Graphics.hpp>
#include <iostream>
#include "login.hpp"
#include "loading.hpp"
#include "HighScoreManager.hpp"

bool loadFont(sf::Font& font) {
    if (!font.loadFromFile("font/arial.ttf")) {
        std::cerr << "Failed to load font from font/arial.ttf\n";
        return false;
    }
    return true;
}

int main() {
    std::cout << "[main] Program started" << std::endl;

    sf::Font font;
    if (!loadFont(font)) {
        std::cerr << "[main] FATAL: Could not load font\n";
        return -1;
    }

    std::string loggedInUser;
    bool loggedIn = CS::runLoginWindow(font, loggedInUser);

    if (!loggedIn) {
        std::cout << "[main] Login canceled or failed. Exiting...\n";
        return 0;
    }

    std::cout << "[main] Before showLoadingScreen()" << std::endl;
    CS::showLoadingScreen(font);
    std::cout << "[main] After showLoadingScreen()" << std::endl;

    std::cout << "[main] Welcome, " << loggedInUser << std::endl;

    int bestScore = 0;

    try {
        std::cout << "[main] Creating HighScoreManager..." << std::endl;
        HighScoreManager highScore("highscores.txt");

        std::cout << "[main] Updating score..." << std::endl;
        int simulatedScore = 8700;
        highScore.updateScore(loggedInUser, simulatedScore);

        std::cout << "[main] Getting high score..." << std::endl;
        bestScore = highScore.getHighScore(loggedInUser);

        std::cout << "[main] High score retrieved: " << bestScore << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[main] Exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "[main] Unknown error occurred in HighScoreManager.\n";
        return -1;
    }

    std::cout << "[main] Creating high score window..." << std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 600), "High Score Display");

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("High Score: " + std::to_string(bestScore));
    scoreText.setPosition(100, 100);

    std::cout << "[main] Entering event loop..." << std::endl;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                std::cout << "[main] Closing window" << std::endl;
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(scoreText);
        window.display();

        sf::sleep(sf::milliseconds(16));  // Prevent high CPU usage
    }

    std::cout << "[main] Program ended" << std::endl;
    return 0;
}
