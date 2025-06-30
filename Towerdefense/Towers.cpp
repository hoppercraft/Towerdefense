#include "Towers.h"
#include "Bullet.h"
#include"iostream"

Tower::Tower(float a, float b) {
    body.setSize({ 10.f, 15.f });
    body.setFillColor(sf::Color(0x6464C8FF));
    body.setOrigin({ body.getGeometricCenter().x, head.getRadius() });
    body.setPosition({ a, b });

    head.setRadius(7.f);
    head.setFillColor(sf::Color(0x0000FFFF));
    head.setOrigin(head.getGeometricCenter());
    head.setPosition(body.getPosition());
    Towerrange.setRadius(50.f);
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    Towerrange.setOutlineColor(sf::Color(0x80808000));
    Towerrange.setOutlineThickness(1.f);
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setPosition(body.getPosition());
}
void Tower::setangle(float x) {
    sf::Angle a = sf::radians(x);
    body.setRotation(a);
}
sf::Angle Tower::gettowerangle() {
    return head.getRotation();
}
void Tower::setfillcolordefault() {
    body.setFillColor(sf::Color(0x6464C8FF));
    head.setFillColor(sf::Color(0x0000FFFF));
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
}
void Tower::showrange() {
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}
void Tower::setfillcolorlight() {
    body.setFillColor(sf::Color(0x6464C855));
    head.setFillColor(sf::Color(0x0000FF55));
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}
sf::FloatRect Tower::getGlobalBounds(){
    return head.getGlobalBounds();
}

sf::FloatRect Tower::getRange() {
    return Towerrange.getGlobalBounds();
}
void Tower::setfillcolorred() {
    body.setFillColor(sf::Color(0xFF64C855));
    head.setFillColor(sf::Color(0xFF00FF55));
    Towerrange.setFillColor(sf::Color(0xFF000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}


sf::Vector2f Tower::gettowerposition() {
    return body.getPosition();
}


void Tower::setposition(sf::Vector2f position) {
    body.setPosition(position);
    head.setPosition(body.getPosition());
    Towerrange.setPosition(body.getPosition());
}

void Tower::draw(sf::RenderWindow& window) const {
    for (const auto& bullet : bullets)
        bullet.draw(window);
    window.draw(body);
    window.draw(head);
    window.draw(Towerrange);
}

bool Tower::contain(sf::Vector2f mousepos) {
    return head.getGlobalBounds().contains(mousepos) || body.getGlobalBounds().contains(mousepos);
}

bool Tower::isInRange(sf::Vector2f other,float range) {
    sf::Vector2f center = body.getPosition();
    float dx = center.x - other.x;
    float dy = center.y - other.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance <= range) {
        return true;
    }
    else
        return false;
}

void Tower::tryShoot(std::vector<Enemy>& enemies) {
    if (Tower::fireCooldown.getElapsedTime().asSeconds() >= fireRate) {
        for (auto& enemy : enemies) {
            if (isInRange(enemy.getposition(),50.f)) {
                bullets.emplace_back(body.getPosition(), enemy.getposition());
                fireCooldown.restart();
                break;
            }
        }
    }
}

void Tower::updateBullets(float dt) {
    for (auto& bullet : bullets) {
        bullet.update(dt);
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.reachedTarget(); }), bullets.end());
}

