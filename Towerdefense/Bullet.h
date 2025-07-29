#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"

enum class BulletType {
    BASIC,
    TESLA
};

class Bullet {
public:
    Bullet(sf::Vector2f startPos, Enemy* enemy, BulletType type = BulletType::BASIC);
    Enemy* targetEnemy;
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool reachedTarget() const;
    sf::Vector2f getPosition() const;
    void hitenemy();
    bool isMoving() const;
    BulletType getBulletType() const { return bulletType; }

private:
    BulletType bulletType;
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f target;
    sf::Vector2f velocity;
    float speed;
    int damage;

    // Tesla bullet specific components
    sf::Clock animationClock;
    std::vector<sf::CircleShape> sparkles;

    void initializeBullet();
    void updateTeslaBullet(float deltaTime);
};