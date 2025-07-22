#include "Towers.h"
#include "Bullet.h"
#include "enemy.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// Base Tower Constructor - removed virtual function call
Tower::Tower(float a, float b, TowerType type) : towerType(type), position({ a, b }) {
    // Initialize range circle (common to all towers)
    Towerrange.setRadius(50.f);
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    Towerrange.setOutlineColor(sf::Color(0x80808000));
    Towerrange.setOutlineThickness(1.f);
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setPosition({ a, b });

    // Don't call virtual function from constructor
    // Each derived class will call initializeComponents() in its own constructor
}

// Base Tower's default initialization (for BasicTower)
void Tower::initializeComponents() {
    float a = position.x;
    float b = position.y;

    base.setRadius(12.f);
    base.setPointCount(8);
    base.setFillColor(sf::Color(85, 107, 47));
    base.setOutlineThickness(1.f);
    base.setOutlineColor(sf::Color(60, 75, 35));
    base.setOrigin({ base.getRadius(), base.getRadius() });
    base.setPosition({ a, b });

    body.setSize({ 16.f, 20.f });
    body.setOutlineThickness(1.f);
    body.setFillColor(sf::Color(107, 142, 35));
    body.setOutlineColor(sf::Color(85, 107, 47));
    body.setOrigin({ 8.f, 10.f });
    body.setPosition({ a, b });

    head.setRadius(8.f);
    head.setOutlineThickness(1.f);
    head.setFillColor(sf::Color(124, 152, 70));
    head.setOutlineColor(sf::Color(85, 107, 47));
    head.setOrigin({ head.getRadius(), head.getRadius() });
    head.setPosition({ a, b - 2.f });\


    barrel.setSize({ 4.f, 18.f }); // Width, Height
    barrel.setFillColor(sf::Color(64, 64, 64, 255)); // Fully opaque dark gray
    barrel.setOutlineThickness(0.5f);
    barrel.setOutlineColor(sf::Color(32, 32, 32)); // Darker gray
    barrel.setOrigin({ 2.f, 2.f }); // Center horizontally, at the back vertically
    barrel.setPosition({ a, b - 2.f }); // Same as head position

    // Sight/targeting device (small rectangle on top of head)
    sight.setSize({ 2.f, 4.f });
    sight.setFillColor(sf::Color(139, 69, 19)); // Brown
    sight.setOrigin({ 1.f, 2.f }); // Center origin
    sight.setPosition({ a, b - 2.f });
}

void Tower::setangle(float x) {
    sf::Angle a = sf::radians(x);
    body.setRotation(a);
    barrel.setRotation(a);
    sight.setRotation(a);
}

sf::Angle Tower::gettowerangle() {
    return head.getRotation();
}

void Tower::setfillcolordefault() {
    body.setFillColor(sf::Color(107, 142, 35, 255));
    body.setOutlineColor(sf::Color(85, 107, 47, 255));
    head.setFillColor(sf::Color(124, 152, 70, 255));
    head.setOutlineColor(sf::Color(85, 107, 47, 255));
    base.setFillColor(sf::Color(0x556b2fFF));
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
    barrel.setFillColor(sf::Color(64, 64, 64, 255));
    barrel.setOutlineColor(sf::Color(32, 32, 32, 255));
    sight.setFillColor(sf::Color(139, 69, 19, 255));
}

void Tower::showrange() {
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}

void Tower::setfillcolorlight() {
    body.setFillColor(sf::Color(107, 142, 35, 100));
    body.setOutlineColor(sf::Color(85, 107, 47, 100));
    head.setFillColor(sf::Color(124, 152, 70, 100));
    head.setOutlineColor(sf::Color(85, 107, 47, 100));
    base.setFillColor(sf::Color(0x556b2f55));
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
    barrel.setFillColor(sf::Color(64, 64, 64, 100));
    barrel.setOutlineColor(sf::Color(32, 32, 32, 100));
    sight.setFillColor(sf::Color(139, 69, 19, 100));
}

void Tower::setfillcolorred() {
    body.setFillColor(sf::Color(200, 142, 35, 100));
    body.setOutlineColor(sf::Color(200, 107, 47, 100));
    head.setFillColor(sf::Color(200, 152, 70, 100));
    head.setOutlineColor(sf::Color(200, 107, 47, 100));
    base.setFillColor(sf::Color(0xFF6b2f55));
    Towerrange.setFillColor(sf::Color(0xFF000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
    barrel.setFillColor(sf::Color(200, 64, 64, 100));
    barrel.setOutlineColor(sf::Color(200, 32, 32, 100));
    sight.setFillColor(sf::Color(200, 69, 19, 100));
}

sf::Vector2f Tower::gettowerposition() {
    return body.getPosition();
}

float Tower::getradius() {
    return head.getRadius();
}

void Tower::setposition(sf::Vector2f position) {
    this->position = position; // Update stored position
    body.setPosition(position);
    head.setPosition({ position.x, position.y - 2.f });
    Towerrange.setPosition(position);
    base.setPosition(position);
    barrel.setPosition({ position.x, position.y - 2.f });
    sight.setPosition({ position.x, position.y - 2.f });
}

void Tower::draw(sf::RenderWindow& window) const {
    for (const auto& bullet : bullets)
        bullet.draw(window);
    window.draw(base);
    window.draw(body);
    window.draw(head);
    window.draw(Towerrange);
    window.draw(barrel);
    window.draw(sight);
}

bool Tower::contain(sf::Vector2f mousepos) {
    return head.getGlobalBounds().contains(mousepos) || body.getGlobalBounds().contains(mousepos);
}

float Tower::getrange() {
    return Towerrange.getRadius();
}

bool Tower::isInRange(sf::Vector2f other, float range) {
    sf::Vector2f center = body.getPosition();
    float dx = center.x - other.x;
    float dy = center.y - other.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= range;
}

void Tower::tryShoot(std::vector<Enemy*>& enemies) {
    if (fireCooldown.getElapsedTime().asSeconds() >= fireRate) {
        for (auto& enemy : enemies) {
            if (isInRange(enemy->getposition(), 50.f)) {
                bullets.emplace_back(body.getPosition(), enemy, BulletType::BASIC);
                fireCooldown.restart();
                break;
            }
        }
    }
}

void Tower::updateBullets(float dt) {
    for (auto& bullet : bullets) {
        bullet.update(dt);
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.reachedTarget() || !b.isMoving(); }), bullets.end());
}

// BasicTower Implementation
BasicTower::BasicTower(float a, float b) : Tower(a, b, TowerType::BASIC) {
    // Call initializeComponents() after the base constructor
    initializeComponents();
}

void BasicTower::initializeComponents() {
    // Use the base Tower initialization for BasicTower
    Tower::initializeComponents();
}

void BasicTower::draw(sf::RenderWindow& window) const {
    // Use base Tower draw method
    Tower::draw(window);
}

// TeslaTower Implementation
TeslaTower::TeslaTower(float a, float b) : Tower(a, b, TowerType::TESLA) {
    // Set Tesla-specific properties
    range = 75.0f; // Tesla towers have longer range
    fireRate = 2.0f; // Tesla towers fire slower (every 2 seconds)

    // Call initializeComponents() after the base constructor
    initializeComponents();
}

void TeslaTower::initializeComponents() {
    float a = position.x;
    float b = position.y;

    // Tesla tower base (larger than basic tower) - NOW RED
    teslaBase.setRadius(15.f);
    teslaBase.setPointCount(6); // Hexagonal base
    teslaBase.setFillColor(sf::Color(220, 20, 60)); // Crimson red
    teslaBase.setOutlineThickness(2.f);
    teslaBase.setOutlineColor(sf::Color(139, 0, 0)); // Dark red
    teslaBase.setOrigin({ teslaBase.getRadius(), teslaBase.getRadius() });
    teslaBase.setPosition({ a, b });

    // Tesla coil structure (multiple rings)
    teslaCoil1.setRadius(8.f);
    teslaCoil1.setFillColor(sf::Color(100, 149, 237)); // Cornflower blue
    teslaCoil1.setOutlineThickness(1.f);
    teslaCoil1.setOutlineColor(sf::Color(72, 61, 139)); // Dark slate blue
    teslaCoil1.setOrigin({ teslaCoil1.getRadius(), teslaCoil1.getRadius() });
    teslaCoil1.setPosition({ a, b - 8.f });

    teslaCoil2.setRadius(6.f);
    teslaCoil2.setFillColor(sf::Color(138, 43, 226)); // Blue violet
    teslaCoil2.setOutlineThickness(1.f);
    teslaCoil2.setOutlineColor(sf::Color(75, 0, 130)); // Indigo
    teslaCoil2.setOrigin({ teslaCoil2.getRadius(), teslaCoil2.getRadius() });
    teslaCoil2.setPosition({ a, b - 16.f });

    teslaCoil3.setRadius(4.f);
    teslaCoil3.setFillColor(sf::Color(147, 112, 219)); // Medium purple
    teslaCoil3.setOutlineThickness(1.f);
    teslaCoil3.setOutlineColor(sf::Color(102, 51, 153)); // Rebecca purple
    teslaCoil3.setOrigin({ teslaCoil3.getRadius(), teslaCoil3.getRadius() });
    teslaCoil3.setPosition({ a, b - 24.f });

    // Tesla top (sphere)
    teslaTop.setRadius(5.f);
    teslaTop.setFillColor(sf::Color(255, 255, 255)); // White
    teslaTop.setOutlineThickness(1.f);
    teslaTop.setOutlineColor(sf::Color(220, 220, 220)); // Gainsboro
    teslaTop.setOrigin({ teslaTop.getRadius(), teslaTop.getRadius() });
    teslaTop.setPosition({ a, b - 30.f });

    // Central rod
    teslaRod.setSize({ 2.f, 35.f });
    teslaRod.setFillColor(sf::Color(192, 192, 192)); // Silver
    teslaRod.setOutlineThickness(0.5f);
    teslaRod.setOutlineColor(sf::Color(169, 169, 169)); // Dark gray
    teslaRod.setOrigin({ 1.f, 0.f });
    teslaRod.setPosition({ a, b - 2.f });

    // Initialize lightning arcs (will be updated dynamically)
    lightningArcs.resize(3);
    for (auto& arc : lightningArcs) {
        arc.setSize({ 1.f, 10.f });
        arc.setFillColor(sf::Color(255, 255, 0, 150)); // Semi-transparent yellow
    }

    // Update the Towerrange for Tesla tower
    Towerrange.setRadius(range);
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    Towerrange.setPosition({ a, b });

    std::cout << "TeslaTower components initialized at position: " << a << ", " << b << std::endl;
}

void TeslaTower::setangle(float x) {
    // Tesla towers don't rotate - they are static structures
    // Override to prevent rotation of Tesla components
    // Do nothing or optionally store the angle for other purposes
    angle = x;
}

void TeslaTower::setposition(sf::Vector2f pos) {
    position = pos;

    teslaBase.setPosition(pos);
    teslaCoil1.setPosition({ pos.x, pos.y - 8.f });
    teslaCoil2.setPosition({ pos.x, pos.y - 16.f });
    teslaCoil3.setPosition({ pos.x, pos.y - 24.f });
    teslaTop.setPosition({ pos.x, pos.y - 30.f });
    teslaRod.setPosition({ pos.x, pos.y - 2.f });

    Towerrange.setPosition(pos);

    updateLightning();
}

bool TeslaTower::contain(sf::Vector2f mousepos) {
    return teslaBase.getGlobalBounds().contains(mousepos) ||
        teslaCoil1.getGlobalBounds().contains(mousepos) ||
        teslaTop.getGlobalBounds().contains(mousepos);
}

void TeslaTower::updateLightning() {
    // Create dynamic lightning effects
    for (size_t i = 0; i < lightningArcs.size(); ++i) {
        float angle = (i * 120.f + lightningClock.getElapsedTime().asSeconds() * 100.f);
        float x = position.x + std::cos(angle * 3.14159f / 180.f) * (10.f + i * 3.f);
        float y = position.y - 20.f + std::sin(angle * 3.14159f / 180.f) * 5.f;

        lightningArcs[i].setPosition({ x, y });
        lightningArcs[i].setRotation(sf::degrees(angle));

        // Flicker effect
        int alpha = 100 + (int)(55 * std::sin(lightningClock.getElapsedTime().asSeconds() * 10.f + i));
        lightningArcs[i].setFillColor(sf::Color(255, 255, 0, alpha));
    }
}

void TeslaTower::draw(sf::RenderWindow& window) const {
    // Draw bullets first
    for (const auto& bullet : bullets)
        bullet.draw(window);

    // Draw Tesla tower components (NO BARREL!)
    window.draw(Towerrange);
    window.draw(teslaBase);
    window.draw(teslaRod);
    window.draw(teslaCoil1);
    window.draw(teslaCoil2);
    window.draw(teslaCoil3);
    window.draw(teslaTop);

    // Draw lightning effects
    for (const auto& arc : lightningArcs) {
        window.draw(arc);
    }
}

void TeslaTower::tryShoot(std::vector<Enemy*>& enemies) {
    if (fireCooldown.getElapsedTime().asSeconds() >= fireRate) {
        // Tesla tower shoots ALL enemies in range (not just first one)
        bool shotFired = false;

        for (auto& enemy : enemies) {
            if (isInRange(enemy->getposition(), range)) {
                bullets.emplace_back(teslaTop.getPosition(), enemy, BulletType::TESLA);
                shotFired = true;
            }
        }

        if (shotFired) {
            fireCooldown.restart();
            updateLightning(); // Update lightning on shooting
        }
    }
}

void TeslaTower::setfillcolordefault() {
    teslaBase.setFillColor(sf::Color(220, 20, 60, 255)); // Crimson red
    teslaBase.setOutlineColor(sf::Color(139, 0, 0, 255)); // Dark red
    teslaCoil1.setFillColor(sf::Color(100, 149, 237, 255));
    teslaCoil2.setFillColor(sf::Color(138, 43, 226, 255));
    teslaCoil3.setFillColor(sf::Color(147, 112, 219, 255));
    teslaTop.setFillColor(sf::Color(255, 255, 255, 255));
    teslaRod.setFillColor(sf::Color(192, 192, 192, 255));
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
}

void TeslaTower::setfillcolorlight() {
    teslaBase.setFillColor(sf::Color(220, 20, 60, 100)); // Crimson red with transparency
    teslaBase.setOutlineColor(sf::Color(139, 0, 0, 100)); // Dark red with transparency
    teslaCoil1.setFillColor(sf::Color(100, 149, 237, 100));
    teslaCoil2.setFillColor(sf::Color(138, 43, 226, 100));
    teslaCoil3.setFillColor(sf::Color(147, 112, 219, 100));
    teslaTop.setFillColor(sf::Color(255, 255, 255, 100));
    teslaRod.setFillColor(sf::Color(192, 192, 192, 100));
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}

void TeslaTower::setfillcolorred() {
    teslaBase.setFillColor(sf::Color(255, 50, 50, 100)); // Brighter red with transparency
    teslaBase.setOutlineColor(sf::Color(200, 0, 0, 100)); // Bright red outline with transparency
    teslaCoil1.setFillColor(sf::Color(255, 149, 237, 100));
    teslaCoil2.setFillColor(sf::Color(255, 43, 226, 100));
    teslaCoil3.setFillColor(sf::Color(255, 112, 219, 100));
    teslaTop.setFillColor(sf::Color(255, 200, 200, 100));
    teslaRod.setFillColor(sf::Color(255, 192, 192, 100));
    Towerrange.setFillColor(sf::Color(0xFF000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}