#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"
#include"Bullet.h"
class Tower {
public:
    Tower(float a = 0, float b = 0);

    void setfillcolordefault();
    void setfillcolorlight();
    void setfillcolorred();
    sf::Vector2f gettowerposition();
    void setposition(sf::Vector2f position);
    void draw(sf::RenderWindow& window) const;
    bool contain(sf::Vector2f mousepos);
    void showrange();
    void setangle(float x);
    sf::Angle gettowerangle();
    void tryShoot(std::vector<Enemy*>& enemies);
    void updateBullets(float dt);
    bool isInRange(sf::Vector2f other,float range);
    float getrange();
    float getradius();
protected:
    sf::CircleShape Towerrange;
    std::vector<Bullet> bullets;
    sf::Clock fireCooldown;
    float fireRate = 0.5f;
    sf::CircleShape base;              // Hexagonal base platform
    sf::RectangleShape body;           // Main turret body
    sf::CircleShape head;              // Turret head/cabin
    sf::RectangleShape sight;          // Targeting sight
    std::vector<sf::RectangleShape> barrels;    // Multiple cannon barrels
    sf::RectangleShape barrel;
};

