#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"
#include "Enemy.h"

class Tower {
public:
    Tower(sf::Vector2f position);
    void update(float deltaTime, const Enemy& enemy, std::vector<Bullet>& bullets);
    void draw(sf::RenderWindow& window) const;

private:
    sf::RectangleShape body;
    sf::CircleShape head;
    sf::Vector2f position;
    float fireCooldown;
};