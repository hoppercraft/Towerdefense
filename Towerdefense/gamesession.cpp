#include <SFML/Graphics.hpp>
#include "gamesession.h"
#include"GameConstants.h"
PlayerInfo::PlayerInfo(int coin, int health)
    : coins(coin), health(health), cointext(font, std::to_string(coin), 80), hearttext(font, std::to_string(health), 80) {
    hearttext.setScale({ 0.105f, 0.105f });
    hearttext.setFillColor(sf::Color::White);
    hearttext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + 30 ,Game::MAP_HEIGHT * Game::TILE_SIZE - 15 });
    heartShape.setPointCount(9);

    // Smaller coordinates (~20x20 heart)
    heartShape.setPoint(0, sf::Vector2f(11.f, -4.f));   // Bottom point
    heartShape.setPoint(1, sf::Vector2f(18.f, 4.f));   // Right low
    heartShape.setPoint(2, sf::Vector2f(20.f, 8.f));   // Right curve outer
    heartShape.setPoint(3, sf::Vector2f(15.f, 15.f));  // Top right bump
    heartShape.setPoint(4, sf::Vector2f(10.f, 12.f));  // Center dip
    heartShape.setPoint(5, sf::Vector2f(5.f, 15.f));   // Top left bump
    heartShape.setPoint(6, sf::Vector2f(0.f, 8.f));    // Left curve outer
    heartShape.setPoint(7, sf::Vector2f(2.f, 4.f));
    heartShape.setPoint(8, sf::Vector2f(9.f, -4.f));
    heartShape.setRotation(sf::degrees(180.f));
    heartShape.setScale({ 0.6f,0.6f });
    // Style it
    heartShape.setFillColor(sf::Color(255, 50, 50)); // Red heart
    heartShape.setOutlineThickness(1.0f);
    heartShape.setOutlineColor(sf::Color::Black);

    // Position it next to heart text
    heartShape.setPosition({
        hearttext.getPosition().x - 4,
        hearttext.getPosition().y + 10
        });
    font.openFromFile("ARIAL.ttf");
    cointext.setScale({ 0.105f, 0.105f });
    cointext.setFillColor(sf::Color::White);
    cointext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + 30 ,Game::MAP_HEIGHT * Game::TILE_SIZE - 30 });
    coinShape.setRadius(5.f);
    coinShape.setFillColor(sf::Color(255, 215, 0));  // Gold color
    coinShape.setPointCount(10);
    coinShape.setOutlineColor(sf::Color(218, 165, 32));  // Darker gold
    coinShape.setOutlineThickness(1.5f);
    coinShape.setPosition({ cointext.getPosition().x - 15,cointext.getPosition().y });
}

void PlayerInfo::draw(sf::RenderWindow& window) {
    window.draw(cointext);
    window.draw(coinShape);
    window.draw(hearttext);
    window.draw(heartShape);
}