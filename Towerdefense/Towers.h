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
    void setposition(sf::Vector2f position);
    void draw(sf::RenderWindow& window) const;
    bool contain(sf::Vector2f mousepos);
    void showrange();
    void setangle(float x);
    sf::Angle gettowerangle();
    void tryShoot(std::vector<Enemy*>& enemies);
    void updateBullets(float dt);
    bool isInRange(sf::Vector2f other, float range);
    float getrange();
    float getradius();
    sf::Vector2f gettowerposition();
protected:
    sf::CircleShape Towerrange;
    std::vector<Bullet> bullets;
    sf::Clock fireCooldown;
    float fireRate = 1.f;
    sf::CircleShape base;
    sf::RectangleShape body;
    sf::CircleShape head;
    sf::RectangleShape sight;
    std::vector<sf::RectangleShape> barrels;
    sf::RectangleShape barrel;
};

class Boat : public Tower {
public:
    Boat(float x = 100, float y = 100);
    void draw(sf::RenderWindow& window);
private:
    sf::ConvexShape boat;
    sf::RectangleShape flag;
    sf::RectangleShape cannonL;
    sf::RectangleShape cannonR;
    sf::CircleShape skull;
    sf::RectangleShape crossbone1;
    sf::RectangleShape crossbone2;
    sf::Vector2f gettowerposition();
};