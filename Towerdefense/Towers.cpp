// Enhanced Tower.cpp - Fixed visibility issues (SFML 3.0 compatible)
#include "Towers.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <stdexcept>

Tower::Tower(float a, float b) : range(75.0f), fireRate(1.0f), timeSinceLastShot(0.0f), bulletDamage(25.0f) {
    // Base platform (hexagonal-ish base)
    base.setRadius(12.f);
    base.setPointCount(8); // Octagonal base
    base.setFillColor(sf::Color(85, 107, 47)); // Dark olive green
    base.setOutlineThickness(1.f);
    base.setOutlineColor(sf::Color(60, 75, 35)); // Darker green outline
    base.setOrigin({ base.getRadius(), base.getRadius() });
    base.setPosition({ a, b });

    // Main turret body (larger rectangular body)
    body.setSize({ 16.f, 20.f });
    body.setFillColor(sf::Color(107, 142, 35)); // Olive drab
    body.setOutlineThickness(1.f);
    body.setOutlineColor(sf::Color(85, 107, 47)); // Darker outline
    body.setOrigin({ 8.f, 10.f }); // Center of rectangle
    body.setPosition({ a, b });

    // Turret head/cabin (rounded top section) - This will be the rotation center
    head.setRadius(8.f);
    head.setFillColor(sf::Color(124, 152, 70)); // Lighter olive
    head.setOutlineThickness(1.f);
    head.setOutlineColor(sf::Color(85, 107, 47));
    head.setOrigin({ head.getRadius(), head.getRadius() });
    head.setPosition({ a, b - 2.f }); // Slightly above center

    // FIXED: Single cannon barrel with proper setup for rotation
    sf::RectangleShape barrel;
    barrel.setSize({ 4.f, 18.f }); // Width, Height
    barrel.setFillColor(sf::Color(64, 64, 64, 255)); // Fully opaque dark gray
    barrel.setOutlineThickness(0.5f);
    barrel.setOutlineColor(sf::Color(32, 32, 32)); // Darker gray

    // CRITICAL FIX: Set origin to rotate around the back of the barrel
    // For a barrel that extends rightward when rotation = 0
    barrel.setOrigin({ 2.f, 2.f }); // Center horizontally, at the back vertically
    barrel.setPosition({ a, b - 2.f }); // Same as head position

    barrels.push_back(barrel);

    // Single muzzle flash indicator
    sf::CircleShape muzzle;
    muzzle.setRadius(2.f);
    muzzle.setFillColor(sf::Color(255, 140, 0, 180)); // Orange with some transparency
    muzzle.setOrigin({ muzzle.getRadius(), muzzle.getRadius() });
    muzzle.setPosition({ a, b - 2.f });

    muzzleFlashes.push_back(muzzle);

    // Sight/targeting device (small rectangle on top of head)
    sight.setSize({ 2.f, 4.f });
    sight.setFillColor(sf::Color(139, 69, 19)); // Brown
    sight.setOrigin({ 1.f, 2.f }); // Center origin
    sight.setPosition({ a, b - 2.f });

    // Range circle
    Towerrange.setRadius(range);
    Towerrange.setOrigin({ range, range });
    Towerrange.setOutlineColor(sf::Color(128, 128, 128, 128));
    Towerrange.setOutlineThickness(2.f);
    Towerrange.setFillColor(sf::Color(128, 128, 128, 32));
    Towerrange.setPosition({ a, b });

    // Initialize muzzle flash visibility
    showMuzzleFlash = false;
    muzzleFlashTimer = 0.0f;

    // Initialize rotation
    currentRotation = 0.0f;
    targetRotation = 0.0f;
    rotationSpeed = 240.0f; // degrees per second

    std::cout << "Enhanced Tower created at: " << a << ", " << b << " with range: " << range << std::endl;
}

void Tower::setfillcolordefault() {
    base.setFillColor(sf::Color(85, 107, 47));
    body.setFillColor(sf::Color(107, 142, 35));
    head.setFillColor(sf::Color(124, 152, 70));
    sight.setFillColor(sf::Color(139, 69, 19));

    for (auto& barrel : barrels) {
        barrel.setFillColor(sf::Color(64, 64, 64, 255)); // Ensure full opacity
    }

    Towerrange.setFillColor(sf::Color(128, 128, 128, 32));
    Towerrange.setOutlineColor(sf::Color(128, 128, 128, 128));
}

void Tower::showrange() {
    Towerrange.setFillColor(sf::Color(128, 128, 128, 64));
    Towerrange.setOutlineColor(sf::Color(128, 128, 128, 255));
}

void Tower::setfillcolorlight() {
    base.setFillColor(sf::Color(85, 107, 47, 128));
    body.setFillColor(sf::Color(107, 142, 35, 128));
    head.setFillColor(sf::Color(124, 152, 70, 128));
    sight.setFillColor(sf::Color(139, 69, 19, 128));

    for (auto& barrel : barrels) {
        barrel.setFillColor(sf::Color(64, 64, 64, 128));
    }

    Towerrange.setFillColor(sf::Color(128, 128, 128, 64));
    Towerrange.setOutlineColor(sf::Color(128, 128, 128, 255));
}

void Tower::setfillcolorred() {
    base.setFillColor(sf::Color(255, 85, 85, 128));
    body.setFillColor(sf::Color(255, 107, 107, 128));
    head.setFillColor(sf::Color(255, 124, 124, 128));
    sight.setFillColor(sf::Color(255, 139, 139, 128));

    for (auto& barrel : barrels) {
        barrel.setFillColor(sf::Color(255, 64, 64, 128));
    }

    Towerrange.setFillColor(sf::Color(255, 128, 128, 32));
    Towerrange.setOutlineColor(sf::Color(255, 128, 128, 255));
}

sf::FloatRect Tower::getGlobalBounds() {
    return head.getGlobalBounds();
}

sf::Vector2f Tower::towerpos() {
    return head.getPosition();
}

void Tower::setposition(sf::Vector2f position) {
    sf::Vector2f currentPos = head.getPosition();
    sf::Vector2f offset = position - currentPos;

    base.setPosition(base.getPosition() + offset);
    body.setPosition(body.getPosition() + offset);
    head.setPosition(position);
    sight.setPosition(sight.getPosition() + offset);
    Towerrange.setPosition(position);

    // Update barrel positions relative to new head position
    for (size_t i = 0; i < barrels.size(); ++i) {
        barrels[i].setPosition(barrels[i].getPosition() + offset);
        muzzleFlashes[i].setPosition(muzzleFlashes[i].getPosition() + offset);
    }
}

void Tower::draw(sf::RenderWindow& window, bool showRange) const {
    if (showRange) {
        window.draw(Towerrange);
    }

    // Draw tower components in order (back to front)
    window.draw(base);      // Base platform (doesn't rotate)
    window.draw(body);      // Main body (doesn't rotate)

    // Get the tower center position
    sf::Vector2f towerCenter = head.getPosition();

    // Draw head (doesn't rotate in this version, but could be made to rotate)
    window.draw(head);

    // FIXED: Proper barrel rotation for SFML 3.0
    if (!barrels.empty()) {
        sf::RectangleShape rotatedBarrel = barrels[0];

        // Set position to tower center (rotation point)
        rotatedBarrel.setPosition(towerCenter);

        // SFML 3.0 compatible rotation - use sf::degrees()
        rotatedBarrel.setRotation(sf::degrees(currentRotation));

        // Ensure barrel is fully visible
        rotatedBarrel.setFillColor(sf::Color(64, 64, 64, 255));

        window.draw(rotatedBarrel);
    }

    // Draw muzzle flash if firing
    if (showMuzzleFlash && !muzzleFlashes.empty()) {
        sf::CircleShape rotatedMuzzle = muzzleFlashes[0];

        // Calculate muzzle position at the tip of the barrel
        float angleRad = currentRotation * M_PI / 180.0f;
        float muzzleDistance = 16.0f; // Distance from turret center to barrel tip
        float muzzleOffsetX = muzzleDistance * std::cos(angleRad);
        float muzzleOffsetY = muzzleDistance * std::sin(angleRad);

        sf::Vector2f muzzlePosition = towerCenter + sf::Vector2f{ muzzleOffsetX, muzzleOffsetY };
        rotatedMuzzle.setPosition(muzzlePosition);

        window.draw(rotatedMuzzle);
    }

    // Draw sight with rotation
    sf::RectangleShape rotatedSight = sight;
    rotatedSight.setPosition(towerCenter);
    rotatedSight.setRotation(sf::degrees(currentRotation)); // SFML 3.0 compatible - use sf::degrees
    window.draw(rotatedSight);
}

void Tower::drawBullets(sf::RenderWindow& window) const {
    for (const auto& bullet : bullets) {
        bullet.draw(window);
    }
}

bool Tower::contain(sf::Vector2f mousepos) {
    return head.getGlobalBounds().contains(mousepos) ||
        body.getGlobalBounds().contains(mousepos) ||
        base.getGlobalBounds().contains(mousepos);
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

    // Update muzzle flash timer
    if (showMuzzleFlash) {
        muzzleFlashTimer += deltaTime;
        if (muzzleFlashTimer >= 0.1f) { // Show muzzle flash for 0.1 seconds
            showMuzzleFlash = false;
            muzzleFlashTimer = 0.0f;
        }
    }

    // Find target and update rotation
    Enemy* target = findNearestEnemy(enemies);
    if (target != nullptr) {
        // Calculate angle to target
        sf::Vector2f towerPos = head.getPosition();
        sf::Vector2f enemyPos = target->getPosition();

        float dx = enemyPos.x - towerPos.x;
        float dy = enemyPos.y - towerPos.y;

        // Calculate target angle in degrees (atan2 gives angle from positive x-axis)
        float angleToTarget = std::atan2(dy, dx) * 180.0f / M_PI;

        targetRotation = angleToTarget;

        // Smooth rotation toward target
        float angleDiff = targetRotation - currentRotation;

        // Normalize angle difference to [-180, 180] for shortest rotation path
        while (angleDiff > 180.0f) angleDiff -= 360.0f;
        while (angleDiff < -180.0f) angleDiff += 360.0f;

        // Rotate toward target
        float rotationStep = rotationSpeed * deltaTime;
        if (std::abs(angleDiff) > 1.0f) { // Reduced threshold for smoother rotation
            if (angleDiff > 0) {
                currentRotation += std::min(rotationStep, angleDiff);
            }
            else {
                currentRotation += std::max(-rotationStep, angleDiff);
            }
        }
        else {
            currentRotation = targetRotation; // Snap to target if very close
        }

        // Keep rotation in [0, 360) range for consistency
        while (currentRotation >= 360.0f) currentRotation -= 360.0f;
        while (currentRotation < 0.0f) currentRotation += 360.0f;

        // Debug output to verify rotation is working
        std::cout << "Target angle: " << angleToTarget << ", Current: " << currentRotation << std::endl;
    }

    // Check if we can fire (based on fire rate)
    if (timeSinceLastShot >= 1.0f / fireRate) {
        if (target != nullptr) {
            // Only fire if we're roughly aimed at the target
            float angleDiff = targetRotation - currentRotation;
            while (angleDiff > 180.0f) angleDiff -= 360.0f;
            while (angleDiff < -180.0f) angleDiff += 360.0f;

            if (std::abs(angleDiff) <= 15.0f) { // Increased tolerance for easier firing
                // Calculate firing position from rotated barrel tip
                sf::Vector2f towerPos = head.getPosition();
                float fireAngle = currentRotation * M_PI / 180.0f; // Convert to radians
                sf::Vector2f fireOffset{ 16.0f * std::cos(fireAngle), 16.0f * std::sin(fireAngle) };
                sf::Vector2f firePosition = towerPos + fireOffset;

                // Create and fire bullet from barrel tip
                bullets.emplace_back(firePosition, target->getPosition());
                timeSinceLastShot = 0.0f;

                // Show muzzle flash
                showMuzzleFlash = true;
                muzzleFlashTimer = 0.0f;

                std::cout << "Enhanced Tower FIRED! Rotation: " << currentRotation << " degrees" << std::endl;
            }
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
            if (bullet.reachedTarget()) {
                sf::Vector2f bulletPos = bullet.getPosition();
                sf::Vector2f towerPos = head.getPosition();

                for (auto& enemy : enemies) {
                    if (enemy.isAlive()) {
                        sf::Vector2f enemyPos = enemy.getPosition();

                        float dx_bullet = bulletPos.x - enemyPos.x;
                        float dy_bullet = bulletPos.y - enemyPos.y;
                        float distanceToBullet = std::sqrt(dx_bullet * dx_bullet + dy_bullet * dy_bullet);

                        float dx_tower = enemyPos.x - towerPos.x;
                        float dy_tower = enemyPos.y - towerPos.y;
                        float distanceToTower = std::sqrt(dx_tower * dx_tower + dy_tower * dy_tower);

                        if (distanceToBullet <= 15.0f && distanceToTower <= range * 1.2f) {
                            enemy.takeDamage(bulletDamage);
                            std::cout << "ENHANCED TOWER HIT! Enemy health: " << enemy.getHealth() << std::endl;
                            return true;
                        }
                    }
                }
                return true;
            }
            return false;
            }),
        bullets.end()
    );
}