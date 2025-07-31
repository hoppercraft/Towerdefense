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
    head.setPosition({ a, b - 2.f });

    barrel.setSize({ 4.f, 18.f }); // Width, Height
    barrel.setFillColor(sf::Color(64, 64, 64, 255));
    barrel.setOutlineThickness(0.5f);
    barrel.setOutlineColor(sf::Color(32, 32, 32));
    barrel.setOrigin({ 2.f, 2.f }); // Center horizontally, at the back vertically
    barrel.setPosition({ a, b - 2.f });

    // Sight/targeting device (small rectangle on top of head)
    sight.setSize({ 2.f, 4.f });
    sight.setFillColor(sf::Color(139, 69, 19));
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
    range = 75.0f;
    fireRate = 3.0f;


    initializeComponents();
}

void TeslaTower::initializeComponents() {
    float a = position.x;
    float b = position.y;

    // Tesla tower base platform (green with brown outline) - MADE EVEN SMALLER
    teslaBase.setSize({ 20.f, 6.f });
    teslaBase.setFillColor(sf::Color(107, 142, 35));
    teslaBase.setOutlineThickness(1.f);
    teslaBase.setOutlineColor(sf::Color(139, 69, 19));
    teslaBase.setOrigin({ 10.f, 3.f });
    teslaBase.setPosition({ a, b + 3.f });

    // Central vertical support rod 
    teslaRod.setSize({ 2.f, 18.f });
    teslaRod.setFillColor(sf::Color(128, 128, 128));
    teslaRod.setOutlineThickness(0.5f);
    teslaRod.setOutlineColor(sf::Color(64, 64, 64));
    teslaRod.setOrigin({ 1.f, 0.f });
    teslaRod.setPosition({ a, b - 15.f });

    // Bottom coil section
    teslaCoil1.setSize({ 7.f, 3.f });
    teslaCoil1.setFillColor(sf::Color(184, 115, 51));
    teslaCoil1.setOutlineThickness(0.5f);
    teslaCoil1.setOutlineColor(sf::Color(139, 69, 19));
    teslaCoil1.setOrigin({ 3.5f, 1.5f });
    teslaCoil1.setPosition({ a, b - 4.f });

    // Middle coil section
    teslaCoil2.setSize({ 7.f, 3.f });
    teslaCoil2.setFillColor(sf::Color(184, 115, 51));
    teslaCoil2.setOutlineThickness(0.5f);
    teslaCoil2.setOutlineColor(sf::Color(139, 69, 19));
    teslaCoil2.setOrigin({ 3.5f, 1.5f });
    teslaCoil2.setPosition({ a, b - 8.f });

    // Upper coil section
    teslaCoil3.setSize({ 7.f, 3.f });
    teslaCoil3.setFillColor(sf::Color(184, 115, 51));
    teslaCoil3.setOutlineThickness(0.5f);
    teslaCoil3.setOutlineColor(sf::Color(139, 69, 19));
    teslaCoil3.setOrigin({ 3.5f, 1.5f });
    teslaCoil3.setPosition({ a, b - 12.f }); // Adjusted position

    // Top conductor (cylindrical, metallic gray with highlight) - MADE SMALLER
    teslaTop.setSize({ 12.f, 4.f });
    teslaTop.setFillColor(sf::Color(192, 192, 192));
    teslaTop.setOutlineThickness(1.f);
    teslaTop.setOutlineColor(sf::Color(64, 64, 64));
    teslaTop.setOrigin({ 6.f, 2.f });
    teslaTop.setPosition({ a, b - 17.f });

    // Top conductor highlight (to simulate cylindrical shape)
    teslaTopHighlight.setSize({ 9.f, 0.8f });
    teslaTopHighlight.setFillColor(sf::Color(220, 220, 220));
    teslaTopHighlight.setOrigin({ 4.5f, 0.4f });
    teslaTopHighlight.setPosition({ a, b - 18.f });

    // Tesla tower initialization - no electricity effects
    // Update the Towerrange for Tesla tower - ensure it matches the actual range
    Towerrange.setRadius(range);
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    Towerrange.setPosition({ a, b });

    std::cout << "TeslaTower components initialized at position: " << a << ", " << b << std::endl;
}

void TeslaTower::setangle(float x) {

    angle = x;
}

void TeslaTower::setposition(sf::Vector2f pos) {
    position = pos;

    teslaBase.setPosition({ pos.x, pos.y + 3.f });
    teslaRod.setPosition({ pos.x, pos.y - 15.f });
    teslaCoil1.setPosition({ pos.x, pos.y - 4.f });
    teslaCoil2.setPosition({ pos.x, pos.y - 8.f });
    teslaCoil3.setPosition({ pos.x, pos.y - 12.f });
    teslaTop.setPosition({ pos.x, pos.y - 17.f });
    teslaTopHighlight.setPosition({ pos.x, pos.y - 18.f });

    Towerrange.setPosition(pos);
}

bool TeslaTower::contain(sf::Vector2f mousepos) {
    return teslaBase.getGlobalBounds().contains(mousepos) ||
        teslaTop.getGlobalBounds().contains(mousepos) ||
        teslaCoil1.getGlobalBounds().contains(mousepos) ||
        teslaCoil2.getGlobalBounds().contains(mousepos) ||
        teslaCoil3.getGlobalBounds().contains(mousepos);
}

void TeslaTower::updateLightning() {
    // No lightning effects - Tesla tower is now clean
}

void TeslaTower::draw(sf::RenderWindow& window) const {
    // Draw bullets first
    for (const auto& bullet : bullets)
        bullet.draw(window);

    // Draw Tesla tower components in proper order (back to front)
    window.draw(Towerrange);
    window.draw(teslaBase);
    window.draw(teslaRod);
    window.draw(teslaCoil1);
    window.draw(teslaCoil2);
    window.draw(teslaCoil3);
    window.draw(teslaTop);
    window.draw(teslaTopHighlight);
}

bool TeslaTower::isInRange(sf::Vector2f other, float range) {

    sf::Vector2f center = position;
    float dx = center.x - other.x;
    float dy = center.y - other.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= range;
}

void TeslaTower::tryShoot(std::vector<Enemy*>& enemies) {
    if (fireCooldown.getElapsedTime().asSeconds() >= fireRate) {
        // Tesla tower shoots ALL enemies in range 
        bool shotFired = false;

        for (auto& enemy : enemies) {
            // Use the Tesla tower's specific range that matches the visual circle
            if (isInRange(enemy->getposition(), Towerrange.getRadius())) {
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
    teslaBase.setFillColor(sf::Color(107, 142, 35, 255));
    teslaBase.setOutlineColor(sf::Color(139, 69, 19, 255));
    teslaRod.setFillColor(sf::Color(128, 128, 128, 255));
    teslaRod.setOutlineColor(sf::Color(64, 64, 64, 255));
    teslaCoil1.setFillColor(sf::Color(184, 115, 51, 255));
    teslaCoil2.setFillColor(sf::Color(184, 115, 51, 255));
    teslaCoil3.setFillColor(sf::Color(184, 115, 51, 255));
    teslaTop.setFillColor(sf::Color(192, 192, 192, 255));
    teslaTopHighlight.setFillColor(sf::Color(220, 220, 220, 255));
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
}

void TeslaTower::setfillcolorlight() {
    teslaBase.setFillColor(sf::Color(107, 142, 35, 100));
    teslaBase.setOutlineColor(sf::Color(139, 69, 19, 100));
    teslaRod.setFillColor(sf::Color(128, 128, 128, 100));
    teslaRod.setOutlineColor(sf::Color(64, 64, 64, 100));
    teslaCoil1.setFillColor(sf::Color(184, 115, 51, 100));
    teslaCoil2.setFillColor(sf::Color(184, 115, 51, 100));
    teslaCoil3.setFillColor(sf::Color(184, 115, 51, 100));
    teslaTop.setFillColor(sf::Color(192, 192, 192, 100));
    teslaTopHighlight.setFillColor(sf::Color(220, 220, 220, 100));
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}

void TeslaTower::setfillcolorred() {
    teslaBase.setFillColor(sf::Color(255, 142, 35, 100));
    teslaBase.setOutlineColor(sf::Color(255, 69, 19, 100));
    teslaRod.setFillColor(sf::Color(255, 128, 128, 100));
    teslaRod.setOutlineColor(sf::Color(200, 64, 64, 100));
    teslaCoil1.setFillColor(sf::Color(255, 115, 51, 100));
    teslaCoil2.setFillColor(sf::Color(255, 115, 51, 100));
    teslaCoil3.setFillColor(sf::Color(255, 115, 51, 100));
    teslaTop.setFillColor(sf::Color(255, 192, 192, 100));
    teslaTopHighlight.setFillColor(sf::Color(255, 220, 220, 100));
    Towerrange.setFillColor(sf::Color(0xFF000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}