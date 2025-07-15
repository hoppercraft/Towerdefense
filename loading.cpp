#include "loading.hpp"
#include <thread>
#include <chrono>

namespace CS {

void showLoadingScreen(const sf::Font& font) {
    sf::RenderWindow window(sf::VideoMode(500, 350), "Loading Screen");

    sf::Text loadingText("Loading...", font, 30);
    loadingText.setFillColor(sf::Color::White);
    loadingText.setPosition(180, 150);

    sf::RectangleShape progressBarBack({400, 30});
    progressBarBack.setFillColor(sf::Color(50, 50, 50));
    progressBarBack.setPosition(50, 220);

    sf::RectangleShape progressBarFront({0, 30});
    progressBarFront.setFillColor(sf::Color(100, 200, 100));
    progressBarFront.setPosition(50, 220);

    const int totalSteps = 100;
    int currentStep = 0;

    sf::Clock clock;
    while (window.isOpen() && currentStep <= totalSteps) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 50) {
            currentStep++;
            progressBarFront.setSize({4.f * currentStep, 30});
            clock.restart();
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(loadingText);
        window.draw(progressBarBack);
        window.draw(progressBarFront);
        window.display();

        if (currentStep > totalSteps) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            window.close();
        }
    }
}

} // namespace CS
