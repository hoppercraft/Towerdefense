
#include <SFML/Graphics.hpp>
#include "gamesession.h"
#include"GameConstants.h"
PlayerInfo::PlayerInfo(int coin, int health)
    : coins(coin), health(health), cointext(font, std::to_string(coin), 80), hearttext(font, std::to_string(health), 80), warningtext(font, "", 170) {
    font.openFromFile("ARIAL.ttf");
    hearttext.setScale({ 0.105f, 0.105f });
    hearttext.setFillColor(sf::Color::White);
    hearttext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + 30 ,Game::MAP_HEIGHT * Game::TILE_SIZE - 15 });
    heartShape.setPointCount(9);

    heartShape.setPoint(0, sf::Vector2f(110.f, -40.f));   // Bottom point
    heartShape.setPoint(1, sf::Vector2f(180.f, 40.f));   // Right low
    heartShape.setPoint(2, sf::Vector2f(200.f, 80.f));   // Right curve outer
    heartShape.setPoint(3, sf::Vector2f(150.f, 150.f));  // Top right bump
    heartShape.setPoint(4, sf::Vector2f(100.f, 120.f));  // Center dip
    heartShape.setPoint(5, sf::Vector2f(50.f, 150.f));   // Top left bump
    heartShape.setPoint(6, sf::Vector2f(0.f, 80.f));    // Left curve outer
    heartShape.setPoint(7, sf::Vector2f(20.f, 40.f));
    heartShape.setPoint(8, sf::Vector2f(90.f, -40.f));
    heartShape.setRotation(sf::degrees(180.f));
    heartShape.setScale({ 0.06f,0.06f });
    // Style it
    heartShape.setFillColor(sf::Color(255, 50, 50)); // Red heart
    heartShape.setOutlineThickness(10.0f);
    heartShape.setOutlineColor(sf::Color::Black);

    // Position it next to heart text
    heartShape.setPosition({
        hearttext.getPosition().x - 4,
        hearttext.getPosition().y + 10
        });
    cointext.setScale({ 0.105f, 0.105f });
    cointext.setFillColor(sf::Color::White);
    cointext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + 30 ,Game::MAP_HEIGHT * Game::TILE_SIZE - 30 });
    coinShape.setRadius(5.f);
    coinShape.setFillColor(sf::Color(255, 215, 0));  // Gold color
    coinShape.setPointCount(10);
    coinShape.setOutlineColor(sf::Color(218, 165, 32));  // Darker gold
    coinShape.setOutlineThickness(1.5f);
    coinShape.setPosition({ cointext.getPosition().x - 15,cointext.getPosition().y });

    warningtext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE / 2 - 50, Game::MAP_HEIGHT * Game::TILE_SIZE / 2 - 50 });
    warningtext.setScale({ 0.105f, 0.105f });
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
    if (coins >= cost) {
        return(true);
    }
    else {
        return(false);
    }

}

void PlayerInfo::notEnoughMoneywarning() {
    warningtext.setString("Not enough money");
    timer.restart();
}

bool PlayerInfo::gameover() {
    if (health <= 0) {
        return(true);
    }
    else {
        return(false);
    }
}
