#include "Enemy.h"
#include "GameConstants.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>

Enemy::Enemy() {
    constexpr int startX = 0;
    constexpr int startY = 3;

    // Initialize enemy shape
    shape.setRadius(7.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(sf::Vector2f(
        static_cast<float>(startX * Game::TILE_SIZE) + Game::TILE_SIZE / 2,
        static_cast<float>(startY * Game::TILE_SIZE) + Game::TILE_SIZE / 2
    ));
    shape.setOrigin(sf::Vector2f(
        shape.getRadius(),
        shape.getRadius()
    ));

    // Initialize pathfinding grid
    for (int y = 0; y < Game::MAP_HEIGHT; ++y) {
        for (int x = 0; x < Game::MAP_WIDTH; ++x) {
            visited[y][x] = false;
        }
    }

    findPath(startX, startY);
}

void Enemy::update(float deltaTime) {
    if (currentStep >= path.size()) return;

    const float speed = 100.0f * deltaTime;
    const sf::Vector2f target(
        static_cast<float>(path[currentStep].x * Game::TILE_SIZE) + Game::TILE_SIZE / 2,
        static_cast<float>(path[currentStep].y * Game::TILE_SIZE) + Game::TILE_SIZE / 2
    );

    const sf::Vector2f direction = target - shape.getPosition();
    const float distance = std::hypot(direction.x, direction.y);

    if (distance < 5.0f) {
        shape.setPosition(target);
        currentStep++;
    }
    else {
        shape.move((direction / distance) * speed);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}

void Enemy::findPath(int x, int y) {
    if (x < 0 || y < 0 || x >= Game::MAP_WIDTH || y >= Game::MAP_HEIGHT)
        return;

    if (Game::gameMap[y][x] != Game::Path || visited[y][x])
        return;

    visited[y][x] = true;
    path.emplace_back(x, y);

    for (const auto& dir : directions) {
        findPath(x + dir.x, y + dir.y);
    }

    if (x == 0 && y == 3 && path.size() > 1) {
        path.emplace_back(x + 1, y);
    }
}