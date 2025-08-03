#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"
enum class BulletType {
    BASIC,
    CANNON,
    TESLA
};
class Bullet {
public:
    Bullet(sf::Vector2f startPos,Enemy* enemy, BulletType type = BulletType::BASIC);
    Enemy* targetEnemy;
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool reachedTarget() const;
    sf::Vector2f getPosition() const;
    bool isMoving() const;
    void increasebulletdamage(int dmg);
    BulletType getBulletType() const;

private:
    float speed;
    BulletType bulletType;
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f target;
    sf::Vector2f velocity;
    int bulletdamage=10; 
    sf::Clock animationClock;
    std::vector<sf::CircleShape> sparkles;
    void initializeBullet();
    void updateTeslaBullet(float deltaTime);
};