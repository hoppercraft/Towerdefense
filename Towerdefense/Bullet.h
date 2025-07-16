#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"
class Bullet {
public:
    Bullet(sf::Vector2f startPos,Enemy* enemy);
    Enemy* targetEnemy;
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool reachedTarget() const;
    sf::Vector2f getPosition() const;
    void hitenemy();
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f target;
    sf::Vector2f velocity;
};