#pragma once
#include <SFML/Graphics.hpp>

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
    sf::Vector2f target;
    sf::Vector2f velocity;
};