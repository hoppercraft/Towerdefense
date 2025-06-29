#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Bullet {
public:
    Bullet(sf::Vector2f startPos, sf::Vector2f targetPos);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool reachedTarget() const;
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f targetPosition;
    sf::Vector2f velocity;
};