#include "Bullet.h"
#include "SoundManager.h"
#include <cmath>
#include <iostream>

Bullet::Bullet(sf::Vector2f startPos, Enemy* enemy, BulletType type)
    : targetEnemy(enemy), bulletType(type), position(startPos) {
    target = enemy->getposition();
    sf::Vector2f direction = target - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    initializeBullet();
    if (length != 0) {
        velocity = direction / length * speed;
    }

    // Play shooting sound when bullet is created
    if (bulletType == BulletType::BASIC) {
        SoundManager::getInstance().playBulletSound();
    }
}

void Bullet::initializeBullet() {
    switch (bulletType) {
    case BulletType::BASIC:
        shape.setRadius(3.f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin({ shape.getRadius(), shape.getRadius() });
        speed = 300.f;
        damage = 10;
        break;
    case BulletType::TESLA:
        shape.setRadius(2.f);
        shape.setFillColor(sf::Color(0, 191, 255, 220));
        shape.setOrigin({ shape.getRadius(), shape.getRadius() });
        speed = 250.f;
        damage = 3;
        sparkles.resize(4);
        for (auto& sparkle : sparkles) {
            sparkle.setRadius(1.f);
            sparkle.setFillColor(sf::Color(255, 255, 255, 150));
            sparkle.setOrigin({ 1.f, 1.f });
        }
        break;
    }
    shape.setPosition(position);
}

void Bullet::update(float deltaTime) {
    position += velocity * deltaTime;
    shape.setPosition(position);

    if (bulletType == BulletType::TESLA) {
        updateTeslaBullet(deltaTime);
    }
}

void Bullet::updateTeslaBullet(float deltaTime) {
    float time = animationClock.getElapsedTime().asSeconds();
    int alpha = 150 + (int)(100 * std::sin(time * 8.0f));
    shape.setFillColor(sf::Color(0, 191, 255, alpha));

    for (size_t i = 0; i < sparkles.size(); ++i) {
        float angle = (i * 90.f) + time * 200.f;
        float radius = 5.f + 2.f * std::sin(time * 6.f + i);
        float x = position.x + std::cos(angle * 3.14159f / 180.f) * radius;
        float y = position.y + std::sin(angle * 3.14159f / 180.f) * radius;
        sparkles[i].setPosition({ x, y });

        int sparkleAlpha = 100 + (int)(100 * std::sin(time * 10.f + i * 2.f));
        sparkles[i].setFillColor(sf::Color(255, 255, 255, sparkleAlpha));
    }
}

void Bullet::draw(sf::RenderWindow& window) const {
    if (bulletType == BulletType::TESLA) {
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