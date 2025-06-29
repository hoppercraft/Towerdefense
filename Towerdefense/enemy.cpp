#include "Enemy.h"
#include <cmath>

Enemy::Enemy() : maxHealth(100.0f), currentHealth(100.0f), currentStep(0) {
    int startX = 0, startY = 3;

    // Initialize soldier components instead of simple circle
    initializeSoldierSprite();

    // Position the soldier at starting location
    sf::Vector2f startPos{ static_cast<float>(startX * Game::TILE_SIZE - 10),
                         static_cast<float>(startY * Game::TILE_SIZE + 10) };
    setSoldierPosition(startPos);

    // Initialize health bar
    healthBarBackground.setSize({ 20.0f, 4.0f });
    healthBarBackground.setFillColor(sf::Color::Black);
    healthBarBackground.setOutlineThickness(1.0f);
    healthBarBackground.setOutlineColor(sf::Color::White);

    healthBarForeground.setSize({ 18.0f, 2.0f });
    healthBarForeground.setFillColor(sf::Color::Green);

    // Initialize visited vector
    visited.resize(Game::MAP_HEIGHT, std::vector<bool>(Game::MAP_WIDTH, false));
    findPath(startX, startY);
    updateHealthBar();
}

void Enemy::initializeSoldierSprite() {
    // Helmet (dark green/black)
    helmet.setRadius(6.0f);
    helmet.setFillColor(sf::Color(40, 60, 40)); // Dark green
    helmet.setOrigin({ 6.0f, 6.0f });

    // Goggles (two small circles)
    leftGoggle.setRadius(2.0f);
    leftGoggle.setFillColor(sf::Color(80, 120, 200)); // Blue tint
    leftGoggle.setOrigin({ 2.0f, 2.0f });

    rightGoggle.setRadius(2.0f);
    rightGoggle.setFillColor(sf::Color(80, 120, 200));
    rightGoggle.setOrigin({ 2.0f, 2.0f });

    // Body (rectangular torso in military green)
    body.setSize({ 8.0f, 12.0f });
    body.setFillColor(sf::Color(60, 80, 40)); // Military green
    body.setOrigin({ 4.0f, 6.0f });

    // Arms (two rectangles)
    leftArm.setSize({ 3.0f, 8.0f });
    leftArm.setFillColor(sf::Color(60, 80, 40));
    leftArm.setOrigin({ 1.5f, 4.0f });

    rightArm.setSize({ 3.0f, 8.0f });
    rightArm.setFillColor(sf::Color(60, 80, 40));
    rightArm.setOrigin({ 1.5f, 4.0f });

    // Legs (two rectangles)
    leftLeg.setSize({ 3.0f, 8.0f });
    leftLeg.setFillColor(sf::Color(50, 70, 35)); // Slightly darker green
    leftLeg.setOrigin({ 1.5f, 4.0f });

    rightLeg.setSize({ 3.0f, 8.0f });
    rightLeg.setFillColor(sf::Color(50, 70, 35));
    rightLeg.setOrigin({ 1.5f, 4.0f });

    // Weapon (simple rifle shape)
    weapon.setSize({ 12.0f, 2.0f });
    weapon.setFillColor(sf::Color(40, 40, 40)); // Dark gray
    weapon.setOrigin({ 6.0f, 1.0f });

    // Chest armor/vest detail
    chestArmor.setSize({ 6.0f, 8.0f });
    chestArmor.setFillColor(sf::Color(80, 100, 60)); // Lighter military green
    chestArmor.setOrigin({ 3.0f, 4.0f });
}

void Enemy::setSoldierPosition(sf::Vector2f position) {
    // Position all components relative to the center position
    helmet.setPosition({ position.x, position.y - 8.0f });

    leftGoggle.setPosition({ position.x - 2.5f, position.y - 8.0f });
    rightGoggle.setPosition({ position.x + 2.5f, position.y - 8.0f });

    body.setPosition({ position.x, position.y });
    chestArmor.setPosition({ position.x, position.y - 1.0f });

    leftArm.setPosition({ position.x - 6.0f, position.y - 2.0f });
    rightArm.setPosition({ position.x + 6.0f, position.y - 2.0f });

    leftLeg.setPosition({ position.x - 2.0f, position.y + 8.0f });
    rightLeg.setPosition({ position.x + 2.0f, position.y + 8.0f });

    weapon.setSize({ 12.0f, 2.0f });
    weapon.setPosition({ position.x + 4.0f, position.y - 3.0f });
    weapon.setRotation(sf::degrees(15.0f)); // SFML 3.0 uses sf::degrees() for angles
}

sf::Vector2f Enemy::getSoldierPosition() const {
    return body.getPosition();
}

// Define the static member
const std::vector<sf::Vector2i> Enemy::directions = {
    sf::Vector2i(1, 0), sf::Vector2i(0, 1), sf::Vector2i(-1, 0), sf::Vector2i(0, -1)
};

void Enemy::update(float speed) {
    if (currentStep < path.size()) {
        sf::Vector2f target{ static_cast<float>(path[currentStep].x * Game::TILE_SIZE + 10),
            static_cast<float>(path[currentStep].y * Game::TILE_SIZE + 10) };
        sf::Vector2f current = getSoldierPosition();
        sf::Vector2f delta = target - current;
        float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (dist < speed) {
            setSoldierPosition(target);
            currentStep++;
        }
        else {
            delta /= dist;
            sf::Vector2f newPos = current + (delta * speed);
            setSoldierPosition(newPos);
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) const {
    // Draw soldier components in proper order (back to front)
    window.draw(weapon);      // Draw weapon first (behind soldier)
    window.draw(leftArm);
    window.draw(rightArm);
    window.draw(leftLeg);
    window.draw(rightLeg);
    window.draw(body);
    window.draw(chestArmor);
    window.draw(helmet);
    window.draw(leftGoggle);
    window.draw(rightGoggle);

    // Draw health bar if damaged
    if (currentHealth < maxHealth) {
        const_cast<Enemy*>(this)->updateHealthBar();
        window.draw(healthBarBackground);
        window.draw(healthBarForeground);
    }
}

void Enemy::updateHealthBar() {
    sf::Vector2f soldierPos = getSoldierPosition();

    // Position health bar above soldier
    healthBarBackground.setPosition({ soldierPos.x - 10.0f, soldierPos.y - 20.0f });
    healthBarForeground.setPosition({ soldierPos.x - 9.0f, soldierPos.y - 19.0f });

    // Update health bar size based on current health
    float healthPercentage = currentHealth / maxHealth;
    healthBarForeground.setSize({ 18.0f * healthPercentage, 2.0f });

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
    return getSoldierPosition();
}