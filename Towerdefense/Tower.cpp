#include "Tower.h"

Tower::Tower(sf::Vector2f position) {
    body.setSize({ 10.f, 15.f });
    body.setFillColor(sf::Color(100, 100, 200));
    body.setOrigin(sf::Vector2f(body.getSize().x / 2.f, head.getRadius()));
    body.setPosition(position);

    head.setRadius(7.f);
    head.setFillColor(sf::Color(0x0000FFFF));
    head.setOrigin(sf::Vector2f(head.getRadius(), head.getRadius()));
    head.setPosition(position);

    this->position = position;
    fireCooldown = 0.f;
}

void Tower::update(float deltaTime, const Enemy& enemy, std::vector<Bullet>& bullets) {
    float distance = std::sqrt(
        (enemy.getPosition().x - position.x) * (enemy.getPosition().x - position.x) +
        (enemy.getPosition().y - position.y) * (enemy.getPosition().y - position.y)
    );

    if (distance < 100.f) {
        fireCooldown -= deltaTime;
        if (fireCooldown <= 0.f) {
            bullets.emplace_back(position, enemy.getPosition());
            fireCooldown = 1.f;
        }
    }
}

void Tower::draw(sf::RenderWindow& window) const {
    window.draw(body);
    window.draw(head);
}