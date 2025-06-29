#include "Enemy.h"
#include <cmath>

Enemy::Enemy() : maxHealth(100.0f), currentHealth(100.0f), currentStep(0) {
    int startX = 0, startY = 3;
    shape.setRadius(7);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(sf::Vector2f(static_cast<float>(startX * Game::TILE_SIZE - 10),
        static_cast<float>(startY * Game::TILE_SIZE + 10)));
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));

    // Initialize health bar
    healthBarBackground.setSize(sf::Vector2f(20.0f, 4.0f));
    healthBarBackground.setFillColor(sf::Color::Black);
    healthBarBackground.setOutlineThickness(1.0f);
    healthBarBackground.setOutlineColor(sf::Color::White);

    healthBarForeground.setSize(sf::Vector2f(18.0f, 2.0f));
    healthBarForeground.setFillColor(sf::Color::Green);

    // Initialize visited vector
    visited.resize(Game::MAP_HEIGHT, std::vector<bool>(Game::MAP_WIDTH, false));
    findPath(startX, startY);
    updateHealthBar();
}

// Define the static member
const std::vector<sf::Vector2i> Enemy::directions = {
    sf::Vector2i(1, 0), sf::Vector2i(0, 1), sf::Vector2i(-1, 0), sf::Vector2i(0, -1)
};

void Enemy::update(float speed) {
    if (currentStep < path.size()) {
        sf::Vector2f target(static_cast<float>(path[currentStep].x * Game::TILE_SIZE + 10),
            static_cast<float>(path[currentStep].y * Game::TILE_SIZE + 10));
        sf::Vector2f current = shape.getPosition();
        sf::Vector2f delta = target - current;
        float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (dist < speed) {
            shape.setPosition(target);
            currentStep++;
        }
        else {
            delta /= dist;
            shape.move(delta * speed);
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    if (currentHealth < maxHealth) { // Only draw health bar if damaged
        // Update health bar position and appearance before drawing
        const_cast<Enemy*>(this)->updateHealthBar();
        window.draw(healthBarBackground);
        window.draw(healthBarForeground);
    }
}

void Enemy::updateHealthBar() {
    sf::Vector2f enemyPos = shape.getPosition();

    // Position health bar above enemy
    healthBarBackground.setPosition(sf::Vector2f(enemyPos.x - 10.0f, enemyPos.y - 15.0f));
    healthBarForeground.setPosition(sf::Vector2f(enemyPos.x - 9.0f, enemyPos.y - 14.0f));

    // Update health bar size based on current health
    float healthPercentage = currentHealth / maxHealth;
    healthBarForeground.setSize(sf::Vector2f(18.0f * healthPercentage, 2.0f));

    // Change color based on health percentage
    if (healthPercentage > 0.6f) {
        healthBarForeground.setFillColor(sf::Color::Green);
    }
    else if (healthPercentage > 0.3f) {
        healthBarForeground.setFillColor(sf::Color::Yellow);
    }
    else {
        healthBarForeground.setFillColor(sf::Color::Red);
    }
}

void Enemy::takeDamage(float damage) {
    currentHealth -= damage;
    if (currentHealth < 0.0f) {
        currentHealth = 0.0f;
    }
}

bool Enemy::isAlive() const {
    return currentHealth > 0.0f;
}

float Enemy::getHealth() const {
    return currentHealth;
}

float Enemy::getMaxHealth() const {
    return maxHealth;
}

void Enemy::findPath(int x, int y) {
    if (x < 0 || y < 0 || x >= Game::MAP_WIDTH || y >= Game::MAP_HEIGHT)
        return;
    if (Game::Map1[y][x] != Game::TileType::Path || visited[y][x])
        return;

    visited[y][x] = true;
    path.push_back(sf::Vector2i(x, y));

    // Use the static directions member
    for (const auto& dir : directions)
        findPath(x + dir.x, y + dir.y);

    if (path.size() == 1 || x != path[0].x || y != path[0].y)
        return;

    sf::Vector2i last = path.back();
    path.push_back(sf::Vector2i(last.x + 1, last.y));
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}