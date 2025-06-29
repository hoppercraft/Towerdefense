// Enhanced Towers.h - Header file for military-style turret
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "Bullet.h"
#include "Enemy.h"

class Tower {
public:
    Tower(float a = 0, float b = 0);
    void setfillcolordefault();
    void setfillcolorlight();
    void setfillcolorred();
    sf::Vector2f towerpos();
    void setposition(sf::Vector2f position);
    void draw(sf::RenderWindow& window, bool showRange = false) const;
    bool contain(sf::Vector2f mousepos);
    void showrange();
    sf::FloatRect getGlobalBounds();

    // Automatic firing methods
    void update(float deltaTime, const std::vector<Enemy>& enemies);
    void updateBullets(float deltaTime, std::vector<Enemy>& enemies);
    void drawBullets(sf::RenderWindow& window) const;
    Enemy* findNearestEnemy(const std::vector<Enemy>& enemies);
    bool isEnemyInRange(const Enemy& enemy) const;
    float getRange() const { return range; }

protected:
    // Enhanced tower components
    sf::CircleShape base;              // Hexagonal base platform
    sf::RectangleShape body;           // Main turret body
    sf::CircleShape head;              // Turret head/cabin
    sf::RectangleShape sight;          // Targeting sight
    std::vector<sf::RectangleShape> barrels;    // Multiple cannon barrels
    std::vector<sf::CircleShape> muzzleFlashes; // Muzzle flash effects
    sf::CircleShape Towerrange;        // Range indicator

    // Combat system
    std::vector<Bullet> bullets;
    float range;
    float fireRate;
    float timeSinceLastShot;
    float bulletDamage;

    // Visual effects
    bool showMuzzleFlash;
    float muzzleFlashTimer;

    // Rotation system
    float currentRotation;    // Current rotation angle in degrees
    float targetRotation;     // Target rotation angle in degrees
    float rotationSpeed;      // Rotation speed in degrees per second

    // Math constant
    static constexpr float M_PI = 3.14159265358979323846f;
};