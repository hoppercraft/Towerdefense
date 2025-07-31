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
    head.setPosition({ a, b - 2.f });

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
// NOTE: Add this method declaration to TeslaTower class in header file:
// bool isInRange(sf::Vector2f other, float range) override;

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

    // Tesla tower base platform (green with brown outline) - MADE EVEN SMALLER
    teslaBase.setSize({ 20.f, 6.f }); // Reduced from 28x8 to 20x6
    teslaBase.setFillColor(sf::Color(107, 142, 35)); // Olive green
    teslaBase.setOutlineThickness(1.f);
    teslaBase.setOutlineColor(sf::Color(139, 69, 19)); // Brown outline
    teslaBase.setOrigin({ 10.f, 3.f }); // Updated origin for new size
    teslaBase.setPosition({ a, b + 3.f }); // Adjusted position

    // Central vertical support rod (gray metallic) - MADE SMALLER
    teslaRod.setSize({ 2.f, 18.f }); // Reduced from 3x24 to 2x18
    teslaRod.setFillColor(sf::Color(128, 128, 128)); // Gray
    teslaRod.setOutlineThickness(0.5f);
    teslaRod.setOutlineColor(sf::Color(64, 64, 64)); // Dark gray
    teslaRod.setOrigin({ 1.f, 0.f }); // Updated origin
    teslaRod.setPosition({ a, b - 15.f }); // Adjusted position

    // Coil sections (copper/brown colored spirals) - MADE SMALLER
    // Bottom coil section
    teslaCoil1.setSize({ 7.f, 3.f }); // Reduced from 10x4 to 7x3
    teslaCoil1.setFillColor(sf::Color(184, 115, 51)); // Copper brown
    teslaCoil1.setOutlineThickness(0.5f);
    teslaCoil1.setOutlineColor(sf::Color(139, 69, 19)); // Darker brown
    teslaCoil1.setOrigin({ 3.5f, 1.5f }); // Updated origin
    teslaCoil1.setPosition({ a, b - 4.f }); // Adjusted position

    // Middle coil section
    teslaCoil2.setSize({ 7.f, 3.f }); // Reduced from 10x4 to 7x3
    teslaCoil2.setFillColor(sf::Color(184, 115, 51)); // Copper brown
    teslaCoil2.setOutlineThickness(0.5f);
    teslaCoil2.setOutlineColor(sf::Color(139, 69, 19)); // Darker brown
    teslaCoil2.setOrigin({ 3.5f, 1.5f }); // Updated origin
    teslaCoil2.setPosition({ a, b - 8.f }); // Adjusted position

    // Upper coil section
    teslaCoil3.setSize({ 7.f, 3.f }); // Reduced from 10x4 to 7x3
    teslaCoil3.setFillColor(sf::Color(184, 115, 51)); // Copper brown
    teslaCoil3.setOutlineThickness(0.5f);
    teslaCoil3.setOutlineColor(sf::Color(139, 69, 19)); // Darker brown
    teslaCoil3.setOrigin({ 3.5f, 1.5f }); // Updated origin
    teslaCoil3.setPosition({ a, b - 12.f }); // Adjusted position

    // Top conductor (cylindrical, metallic gray with highlight) - MADE SMALLER
    teslaTop.setSize({ 12.f, 4.f }); // Reduced from 16x5 to 12x4
    teslaTop.setFillColor(sf::Color(192, 192, 192)); // Light gray
    teslaTop.setOutlineThickness(1.f);
    teslaTop.setOutlineColor(sf::Color(64, 64, 64)); // Dark gray
    teslaTop.setOrigin({ 6.f, 2.f }); // Updated origin
    teslaTop.setPosition({ a, b - 17.f }); // Adjusted position

    // Top conductor highlight (to simulate cylindrical shape) - MADE SMALLER
    teslaTopHighlight.setSize({ 9.f, 0.8f }); // Reduced from 12x1 to 9x0.8
    teslaTopHighlight.setFillColor(sf::Color(220, 220, 220)); // Very light gray
    teslaTopHighlight.setOrigin({ 4.5f, 0.4f }); // Updated origin
    teslaTopHighlight.setPosition({ a, b - 18.f }); // Adjusted position

    // Initialize lightning arcs (will be updated dynamically) - MADE SMALLER
    lightningArcs.resize(3);
    for (size_t i = 0; i < lightningArcs.size(); ++i) {
        lightningArcs[i].setSize({ 1.f, 7.f }); // Reduced from 1.5x10 to 1x7
        lightningArcs[i].setFillColor(sf::Color(0, 191, 255, 180)); // Electric blue with transparency
        lightningArcs[i].setOrigin({ 0.5f, 3.5f }); // Updated origin
    }

    // Update the Towerrange for Tesla tower - ensure it matches the actual range
    Towerrange.setRadius(range); // This should be 75.0f for Tesla towers
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

    teslaBase.setPosition({ pos.x, pos.y + 3.f }); // Adjusted for smaller size
    teslaRod.setPosition({ pos.x, pos.y - 15.f }); // Adjusted for smaller size
    teslaCoil1.setPosition({ pos.x, pos.y - 4.f }); // Adjusted for smaller size
    teslaCoil2.setPosition({ pos.x, pos.y - 8.f }); // Adjusted for smaller size
    teslaCoil3.setPosition({ pos.x, pos.y - 12.f }); // Adjusted for smaller size
    teslaTop.setPosition({ pos.x, pos.y - 17.f }); // Adjusted for smaller size
    teslaTopHighlight.setPosition({ pos.x, pos.y - 18.f }); // Adjusted for smaller size

    Towerrange.setPosition(pos);

    updateLightning();
}

bool TeslaTower::contain(sf::Vector2f mousepos) {
    return teslaBase.getGlobalBounds().contains(mousepos) ||
        teslaTop.getGlobalBounds().contains(mousepos) ||
        teslaCoil1.getGlobalBounds().contains(mousepos) ||
        teslaCoil2.getGlobalBounds().contains(mousepos) ||
        teslaCoil3.getGlobalBounds().contains(mousepos);
}

void TeslaTower::updateLightning() {
    // Create dynamic lightning effects emanating from the top conductor
    float time = lightningClock.getElapsedTime().asSeconds();

    for (size_t i = 0; i < lightningArcs.size(); ++i) {
        // Calculate angles for lightning bolts (120 degrees apart)
        float baseAngle = i * 120.f;
        float wobble = std::sin(time * 8.f + i * 2.f) * 15.f; // Add some randomness
        float angle = baseAngle + wobble;

        // Position lightning from the edges of the top conductor - ADJUSTED FOR SMALLER SIZE
        float radius = 7.f; // Reduced from 10.f to 7.f
        float x = position.x + std::cos(angle * 3.14159f / 180.f) * radius;
        float y = position.y - 17.f + std::sin(angle * 3.14159f / 180.f) * radius; // Adjusted for new top position

        lightningArcs[i].setPosition({ x, y });
        lightningArcs[i].setRotation(sf::degrees(angle));

        // Dynamic length and intensity - SHORTER FOR SMALLER TOWER
        float length = 5.f + std::sin(time * 12.f + i) * 3.f; // Reduced from 8+5 to 5+3
        lightningArcs[i].setSize({ 1.f, length });

        // Flicker effect with electric blue color
        int alpha = 120 + (int)(80 * std::sin(time * 15.f + i * 1.5f));
        lightningArcs[i].setFillColor(sf::Color(0, 191, 255, alpha));
    }
}

void TeslaTower::draw(sf::RenderWindow& window) const {
    // Draw bullets first
    for (const auto& bullet : bullets)
        bullet.draw(window);

    // Draw Tesla tower components in proper order (back to front)
    window.draw(Towerrange);
    window.draw(teslaBase);      // Base platform
    window.draw(teslaRod);       // Central support rod
    window.draw(teslaCoil1);     // Bottom coil
    window.draw(teslaCoil2);     // Middle coil
    window.draw(teslaCoil3);     // Top coil
    window.draw(teslaTop);       // Top conductor
    window.draw(teslaTopHighlight); // Highlight on top conductor

    // Draw lightning effects on top
    for (const auto& arc : lightningArcs) {
        window.draw(arc);
    }
}

bool TeslaTower::isInRange(sf::Vector2f other, float range) {
    // Use Tesla tower's position (center of the base) instead of body position
    sf::Vector2f center = position; // Tesla tower uses its stored position
    float dx = center.x - other.x;
    float dy = center.y - other.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= range;
}

void TeslaTower::tryShoot(std::vector<Enemy*>& enemies) {
    if (fireCooldown.getElapsedTime().asSeconds() >= fireRate) {
        // Tesla tower shoots ALL enemies in range (not just first one)
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
    teslaBase.setFillColor(sf::Color(107, 142, 35, 255)); // Olive green
    teslaBase.setOutlineColor(sf::Color(139, 69, 19, 255)); // Brown
    teslaRod.setFillColor(sf::Color(128, 128, 128, 255)); // Gray
    teslaRod.setOutlineColor(sf::Color(64, 64, 64, 255)); // Dark gray
    teslaCoil1.setFillColor(sf::Color(184, 115, 51, 255)); // Copper
    teslaCoil2.setFillColor(sf::Color(184, 115, 51, 255)); // Copper
    teslaCoil3.setFillColor(sf::Color(184, 115, 51, 255)); // Copper
    teslaTop.setFillColor(sf::Color(192, 192, 192, 255)); // Light gray
    teslaTopHighlight.setFillColor(sf::Color(220, 220, 220, 255)); // Very light gray
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
}

void TeslaTower::setfillcolorlight() {
    teslaBase.setFillColor(sf::Color(107, 142, 35, 100)); // Olive green with transparency
    teslaBase.setOutlineColor(sf::Color(139, 69, 19, 100)); // Brown with transparency
    teslaRod.setFillColor(sf::Color(128, 128, 128, 100)); // Gray with transparency
    teslaRod.setOutlineColor(sf::Color(64, 64, 64, 100)); // Dark gray with transparency
    teslaCoil1.setFillColor(sf::Color(184, 115, 51, 100)); // Copper with transparency
    teslaCoil2.setFillColor(sf::Color(184, 115, 51, 100)); // Copper with transparency
    teslaCoil3.setFillColor(sf::Color(184, 115, 51, 100)); // Copper with transparency
    teslaTop.setFillColor(sf::Color(192, 192, 192, 100)); // Light gray with transparency
    teslaTopHighlight.setFillColor(sf::Color(220, 220, 220, 100)); // Very light gray with transparency
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}

void TeslaTower::setfillcolorred() {
    teslaBase.setFillColor(sf::Color(255, 142, 35, 100)); // Orange-red with transparency
    teslaBase.setOutlineColor(sf::Color(255, 69, 19, 100)); // Red-orange with transparency
    teslaRod.setFillColor(sf::Color(255, 128, 128, 100)); // Pink-gray with transparency
    teslaRod.setOutlineColor(sf::Color(200, 64, 64, 100)); // Red-gray with transparency
    teslaCoil1.setFillColor(sf::Color(255, 115, 51, 100)); // Red-copper with transparency
    teslaCoil2.setFillColor(sf::Color(255, 115, 51, 100)); // Red-copper with transparency
    teslaCoil3.setFillColor(sf::Color(255, 115, 51, 100)); // Red-copper with transparency
    teslaTop.setFillColor(sf::Color(255, 192, 192, 100)); // Pink-gray with transparency
    teslaTopHighlight.setFillColor(sf::Color(255, 220, 220, 100)); // Light pink with transparency
    Towerrange.setFillColor(sf::Color(0xFF000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}