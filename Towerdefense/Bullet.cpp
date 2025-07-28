#include "Bullet.h"

Bullet::Bullet(sf::Vector2f startPos, Enemy* enemy) : targetEnemy(enemy) {
    shape.setRadius(3.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
    shape.setPosition(startPos);

    position = startPos;
    target = enemy->getposition();

    sf::Vector2f direction = target - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        velocity = direction / length * 300.f;
    }
}

void Bullet::update(float deltaTime) {
    position += velocity * deltaTime;
    shape.setPosition(position);
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

bool Bullet::reachedTarget() const {
    float distance = std::sqrt(
        (target.x - position.x) * (target.x - position.x) +
        (target.y - position.y) * (target.y - position.y)
    );
    if (distance < 5.f) {
        targetEnemy->Health -= 10;
    }
    return distance < 5.f;
}

bool Bullet::isMoving() const {
    return std::abs(velocity.x) > 0.01f || std::abs(velocity.y) > 0.01f;
}

void Bullet::hitenemy() {
    targetEnemy->Health -= 10;
    targetEnemy->updateHealthbarnev();
}

sf::Vector2f Bullet::getPosition() const { return position; }