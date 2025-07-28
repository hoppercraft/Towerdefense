#include <SFML/Graphics.hpp>
#include "gamesession.h"
#include "GameConstants.h"

PlayerInfo::PlayerInfo(int coin, int health)
    : coins(coin), health(health) {

    // Load font first
    if (!font.loadFromFile("ARIAL.ttf")) {
        // Handle font loading error if needed
    }

    // Setup texts - in SFML 3.0, Text constructor doesn't take font parameter
    cointext.setFont(font);
    cointext.setString(std::to_string(coin));
    cointext.setCharacterSize(80);

    hearttext.setFont(font);
    hearttext.setString(std::to_string(health));
    hearttext.setCharacterSize(80);

    warningtext.setFont(font);
    warningtext.setString("");
    warningtext.setCharacterSize(170);

    // Setup heart text
    hearttext.setScale(sf::Vector2f(0.105f, 0.105f));
    hearttext.setFillColor(sf::Color::White);
    hearttext.setPosition(sf::Vector2f(Game::MAP_WIDTH * Game::TILE_SIZE + 30, Game::MAP_HEIGHT * Game::TILE_SIZE - 15));

    // Setup heart shape
    heartShape.setPointCount(9);
    heartShape.setPoint(0, sf::Vector2f(110.f, -40.f));   // Bottom point
    heartShape.setPoint(1, sf::Vector2f(180.f, 40.f));    // Right low
    heartShape.setPoint(2, sf::Vector2f(200.f, 80.f));    // Right curve outer
    heartShape.setPoint(3, sf::Vector2f(150.f, 150.f));   // Top right bump
    heartShape.setPoint(4, sf::Vector2f(100.f, 120.f));   // Center dip
    heartShape.setPoint(5, sf::Vector2f(50.f, 150.f));    // Top left bump
    heartShape.setPoint(6, sf::Vector2f(0.f, 80.f));      // Left curve outer
    heartShape.setPoint(7, sf::Vector2f(20.f, 40.f));
    heartShape.setPoint(8, sf::Vector2f(90.f, -40.f));

    heartShape.setRotation(sf::degrees(180.f));
    heartShape.setScale(sf::Vector2f(0.06f, 0.06f));
    heartShape.setFillColor(sf::Color(255, 50, 50)); // Red heart
    heartShape.setOutlineThickness(10.0f);
    heartShape.setOutlineColor(sf::Color::Black);
    heartShape.setPosition(sf::Vector2f(
        hearttext.getPosition().x - 4,
        hearttext.getPosition().y + 10
    ));

    // Setup coin text
    cointext.setScale(sf::Vector2f(0.105f, 0.105f));
    cointext.setFillColor(sf::Color::White);
    cointext.setPosition(sf::Vector2f(Game::MAP_WIDTH * Game::TILE_SIZE + 30, Game::MAP_HEIGHT * Game::TILE_SIZE - 30));

    // Setup coin shape
    coinShape.setRadius(5.f);
    coinShape.setFillColor(sf::Color(255, 215, 0));  // Gold color
    coinShape.setPointCount(10);
    coinShape.setOutlineColor(sf::Color(218, 165, 32));  // Darker gold
    coinShape.setOutlineThickness(1.5f);
    coinShape.setPosition(sf::Vector2f(cointext.getPosition().x - 15, cointext.getPosition().y));

    // Setup warning text
    warningtext.setPosition(sf::Vector2f(Game::MAP_WIDTH * Game::TILE_SIZE / 2 - 50, Game::MAP_HEIGHT * Game::TILE_SIZE / 2 - 50));
    warningtext.setScale(sf::Vector2f(0.105f, 0.105f));
    warningtext.setFillColor(sf::Color(255, 0, 0));
    warningtext.setStyle(sf::Text::Bold);
}

void PlayerInfo::draw(sf::RenderWindow& window) {
    window.draw(cointext);
    window.draw(coinShape);
    window.draw(hearttext);
    window.draw(heartShape);
    if (timer.getElapsedTime().asSeconds() <= 1.f) {
        window.draw(warningtext);
    }
}

void PlayerInfo::turrentplaced(int cost) {
    coins -= cost;
    cointext.setString(std::to_string(coins));
}

void PlayerInfo::coinsearned(int m) {
    coins += m;
    cointext.setString(std::to_string(coins));
}

void PlayerInfo::enemypassed(int healthlost) {
    health -= healthlost;
    hearttext.setString(std::to_string(health));
}

bool PlayerInfo::enoughmoney(int cost) {
    return coins >= cost;
}

void PlayerInfo::notEnoughMoneywarning() {
    warningtext.setString("Not enough money");
    timer.restart();
}

bool PlayerInfo::gameover() {
    return health <= 0;
}