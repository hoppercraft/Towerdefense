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
    sf::FloatRect getGlobalBounds();
    sf::FloatRect getRange();
    void setangle(float x);
    sf::Angle gettowerangle();
    void tryShoot(std::vector<Enemy>& enemies);
    void updateBullets(float dt);
    bool isInRange(sf::Vector2f other,float range);
protected:
    sf::RectangleShape body;
    sf::CircleShape head;
    sf::CircleShape Towerrange;
    std::vector<Bullet> bullets;
    sf::Clock fireCooldown;
    float fireRate = 0.5f;
    
};

