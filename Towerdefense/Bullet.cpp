#include "Bullet.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f startPos, Enemy* enemy, BulletType type)
    : targetEnemy(enemy), bulletType(type), position(startPos) {
    target = enemy->getposition();
    sf::Vector2f direction = target - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    initializeBullet();
    if (length != 0) {
        velocity = direction / length * speed;
    }
}

void Bullet::initializeBullet() {
    switch (bulletType) {
    case BulletType::BASIC:
        shape.setRadius(3.f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin({ shape.getRadius(), shape.getRadius() });  // SFML 3.0: use braced initialization
        speed = 300.f;
        damage = 10;
        break;
    case BulletType::TESLA:
        shape.setRadius(2.f);
        shape.setFillColor(sf::Color(0, 191, 255, 220)); // Deep sky blue with transparency
        shape.setOrigin({ shape.getRadius(), shape.getRadius() });  // SFML 3.0: use braced initialization
        speed = 250.f; // Slightly slower than basic bullets
        damage = 3; // Much lower damage per bullet, but hits all enemies
        // Initialize sparkles for Tesla bullet
        sparkles.resize(4);
        for (auto& sparkle : sparkles) {
            sparkle.setRadius(1.f);
            sparkle.setFillColor(sf::Color(255, 255, 255, 150)); // White sparkles
            sparkle.setOrigin({ 1.f, 1.f });  // SFML 3.0: use braced initialization
        }
        break;
    }
    shape.setPosition(position);  // SFML 3.0: sf::Vector2f should work directly
}

void Bullet::update(float deltaTime) {
    position += velocity * deltaTime;
    shape.setPosition(position);  // SFML 3.0: sf::Vector2f should work directly

    if (bulletType == BulletType::TESLA) {
        updateTeslaBullet(deltaTime);
    }
}

void Bullet::updateTeslaBullet(float deltaTime) {
    // Update main bullet color with pulsing effect
    float time = animationClock.getElapsedTime().asSeconds();
    int alpha = 150 + (int)(100 * std::sin(time * 8.0f));
    shape.setFillColor(sf::Color(0, 191, 255, alpha));

    // Update sparkles around the bullet
    for (size_t i = 0; i < sparkles.size(); ++i) {
        float angle = (i * 90.f) + time * 200.f; // Rotate sparkles
        float radius = 5.f + 2.f * std::sin(time * 6.f + i); // Varying distance
        float x = position.x + std::cos(angle * 3.14159f / 180.f) * radius;
        float y = position.y + std::sin(angle * 3.14159f / 180.f) * radius;
        sparkles[i].setPosition({ x, y });  // SFML 3.0: use braced initialization

        // Flickering effect for sparkles
        int sparkleAlpha = 100 + (int)(100 * std::sin(time * 10.f + i * 2.f));
        sparkles[i].setFillColor(sf::Color(255, 255, 255, sparkleAlpha));
    }
}

void Bullet::draw(sf::RenderWindow& window) const {
    if (bulletType == BulletType::TESLA) {
        // Draw sparkles first (behind the main bullet)
        for (const auto& sparkle : sparkles) {
            window.draw(sparkle);
        }
    }
    window.draw(shape);
}

bool Bullet::reachedTarget() const {
    float distance = std::sqrt(
        (target.x - position.x) * (target.x - position.x) +
        (target.y - position.y) * (target.y - position.y)
    );
    if (distance < 5.f && targetEnemy) {
        targetEnemy->Health -= damage;
        targetEnemy->updateHealthbarnev();
    }
    return distance < 5.f;
}

bool Bullet::isMoving() const {
    return std::abs(velocity.x) > 0.01f || std::abs(velocity.y) > 0.01f;
}

void Bullet::hitenemy() {
    if (targetEnemy) {
        targetEnemy->Health -= damage;
        targetEnemy->updateHealthbarnev();
    }
}

sf::Vector2f Bullet::getPosition() const {
    return position;
}