#include "Towers.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Tower::Tower(float a, float b) : range(75.0f), fireRate(1.0f), timeSinceLastShot(0.0f), bulletDamage(25.0f) {
    body.setSize({ 10.f, 15.f });
    body.setFillColor(sf::Color(0x6464C8FF));
    body.setOrigin({ body.getGeometricCenter().x, body.getGeometricCenter().y });
    body.setPosition({ a, b });

    head.setRadius(7.f);
    head.setFillColor(sf::Color(0x0000FFFF));
    head.setOrigin(head.getGeometricCenter());
    head.setPosition({ a, b });

    Towerrange.setRadius(range);
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    Towerrange.setOutlineColor(sf::Color(0x80808080));
    Towerrange.setOutlineThickness(2.f);
    Towerrange.setFillColor(sf::Color(0x80808020));
    Towerrange.setPosition({ a, b });

    std::cout << "Tower created at: " << a << ", " << b << " with range: " << range << std::endl;
}

void Tower::setfillcolordefault() {
    body.setFillColor(sf::Color(0x6464C8FF));
    head.setFillColor(sf::Color(0x0000FFFF));
    Towerrange.setFillColor(sf::Color(0x80808020));
    Towerrange.setOutlineColor(sf::Color(0x80808080));
}

void Tower::showrange() {
    Towerrange.setFillColor(sf::Color(0x80808040));
    Towerrange.setOutlineColor(sf::Color(0x808080FF));
}

void Tower::setfillcolorlight() {
    body.setFillColor(sf::Color(0x6464C855));
    head.setFillColor(sf::Color(0x0000FF55));
    Towerrange.setFillColor(sf::Color(0x80808040));
    Towerrange.setOutlineColor(sf::Color(0x808080FF));
}

sf::FloatRect Tower::getGlobalBounds() {
    return head.getGlobalBounds();
}

void Tower::setfillcolorred() {
    body.setFillColor(sf::Color(0xFF64C855));
    head.setFillColor(sf::Color(0xFF00FF55));
    Towerrange.setFillColor(sf::Color(0xFF808020));
    Towerrange.setOutlineColor(sf::Color(0xFF8080FF));
}

sf::Vector2f Tower::towerpos() {
    return head.getPosition();
}

void Tower::setposition(sf::Vector2f position) {
    body.setPosition(position);
    head.setPosition(position);
    Towerrange.setPosition(position);
}

// Modified draw method with optional range display
void Tower::draw(sf::RenderWindow& window, bool showRange) const {
    if (showRange) {
        window.draw(Towerrange); // Only draw range circle when requested
    }
    window.draw(body);
    window.draw(head);
}

void Tower::drawBullets(sf::RenderWindow& window) const {
    for (const auto& bullet : bullets) {
        bullet.draw(window);
    }
}

bool Tower::contain(sf::Vector2f mousepos) {
    return head.getGlobalBounds().contains(mousepos) || body.getGlobalBounds().contains(mousepos);
}

bool Tower::isEnemyInRange(const Enemy& enemy) const {
    sf::Vector2f towerPos = head.getPosition();
    sf::Vector2f enemyPos = enemy.getPosition();

    float dx = enemyPos.x - towerPos.x;
    float dy = enemyPos.y - towerPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    return distance <= range;
}

Enemy* Tower::findNearestEnemy(const std::vector<Enemy>& enemies) {
    Enemy* nearestEnemy = nullptr;
    float nearestDistance = range + 1.0f;
    sf::Vector2f towerPos = head.getPosition();

    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            sf::Vector2f enemyPos = enemy.getPosition();

            float dx = enemyPos.x - towerPos.x;
            float dy = enemyPos.y - towerPos.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= range && distance < nearestDistance) {
                nearestDistance = distance;
                nearestEnemy = const_cast<Enemy*>(&enemy);
            }
        }
    }

    return nearestEnemy;
}

void Tower::update(float deltaTime, const std::vector<Enemy>& enemies) {
    timeSinceLastShot += deltaTime;

    // Debug: Check if there are enemies
    static float debugTimer = 0.0f;
    debugTimer += deltaTime;

    if (debugTimer >= 2.0f) { // Print debug info every 2 seconds
        std::cout << "Tower at (" << head.getPosition().x << ", " << head.getPosition().y
            << ") - Enemies in game: " << enemies.size()
            << ", Time since last shot: " << timeSinceLastShot << std::endl;
        debugTimer = 0.0f;
    }

    // Check if we can fire (based on fire rate)
    if (timeSinceLastShot >= 1.0f / fireRate) {
        Enemy* target = findNearestEnemy(enemies);
        if (target != nullptr) {
            // Create and fire bullet - SFML 3.0 compatible
            bullets.emplace_back(head.getPosition(), target->getPosition());
            timeSinceLastShot = 0.0f;
            std::cout << "Tower FIRED! Bullet created from (" << head.getPosition().x
                << ", " << head.getPosition().y << ") to ("
                << target->getPosition().x << ", " << target->getPosition().y << ")" << std::endl;
        }
    }
}

void Tower::updateBullets(float deltaTime, std::vector<Enemy>& enemies) {
    // Update all bullets
    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }

    // Check for collisions and remove bullets that hit targets or reached their destination
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [&](const Bullet& bullet) {
            // Check if bullet reached its target
            if (bullet.reachedTarget()) {
                // Find the closest enemy to the bullet and damage it (within tower range)
                sf::Vector2f bulletPos = bullet.getPosition();
                sf::Vector2f towerPos = head.getPosition();

                for (auto& enemy : enemies) {
                    if (enemy.isAlive()) {
                        sf::Vector2f enemyPos = enemy.getPosition();

                        // Check if enemy is close to bullet impact
                        float dx_bullet = bulletPos.x - enemyPos.x;
                        float dy_bullet = bulletPos.y - enemyPos.y;
                        float distanceToBullet = std::sqrt(dx_bullet * dx_bullet + dy_bullet * dy_bullet);

                        // Check if enemy is still within tower range (with tolerance)
                        float dx_tower = enemyPos.x - towerPos.x;
                        float dy_tower = enemyPos.y - towerPos.y;
                        float distanceToTower = std::sqrt(dx_tower * dx_tower + dy_tower * dy_tower);

                        if (distanceToBullet <= 15.0f && distanceToTower <= range * 1.2f) {
                            enemy.takeDamage(bulletDamage);
                            std::cout << "BULLET HIT! Enemy health: " << enemy.getHealth() << std::endl;
                            return true; // Remove this bullet
                        }
                    }
                }
                return true; // Remove bullet even if it didn't hit anything
            }
            return false; // Keep bullet
            }),
        bullets.end()
    );
}