#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"  // Make sure this matches your actual filename
#include "Enemy.h"

class Tower {
public:
    Tower(float a = 0, float b = 0);
    void setfillcolordefault();
    void setfillcolorlight();
    void setfillcolorred();
    sf::Vector2f towerpos();
    void setposition(sf::Vector2f position);
    void draw(sf::RenderWindow& window, bool showRange = false) const;  // Added showRange parameter
    bool contain(sf::Vector2f mousepos);
    void showrange();
    sf::FloatRect getGlobalBounds();

    // New methods for automatic firing
    void update(float deltaTime, const std::vector<Enemy>& enemies);
    void updateBullets(float deltaTime, std::vector<Enemy>& enemies);
    void drawBullets(sf::RenderWindow& window) const;
    Enemy* findNearestEnemy(const std::vector<Enemy>& enemies);
    bool isEnemyInRange(const Enemy& enemy) const;
    float getRange() const { return range; }

protected:
    sf::RectangleShape body;
    sf::CircleShape head;
    sf::CircleShape Towerrange;

    // New members for automatic firing
    std::vector<Bullet> bullets;
    float range;
    float fireRate;        // Bullets per second
    float timeSinceLastShot;
    float bulletDamage;
};