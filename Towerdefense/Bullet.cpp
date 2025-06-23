#include "Bullet.h"

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos) {
    shape.setRadius(3.f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
    shape.setPosition(startPos);

    position = startPos;
    target = targetPos;

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
    return distance < 5.f;
}

sf::Vector2f Bullet::getPosition() const { return position; }