#include "Towers.h"
#include "Bullet.h"
#include"iostream"

Tower::Tower(float a, float b) {
    base.setRadius(12.f);
    base.setPointCount(8);
    base.setFillColor(sf::Color(85, 107, 47));
    base.setOutlineThickness(1.f);
    base.setOutlineColor(sf::Color(60, 75, 35));
    base.setOrigin({ base.getRadius(), base.getRadius() });
    base.setPosition({ a, b });

    body.setSize({ 16.f, 20.f });
    body.setOutlineThickness(1.f);
    body.setFillColor(sf::Color(107, 142, 35));
    body.setOutlineColor(sf::Color(85, 107, 47));
    body.setOrigin({ 8.f, 10.f });
    body.setPosition({ a, b });

    head.setRadius(8.f);
    head.setOutlineThickness(1.f);
    head.setFillColor(sf::Color(124, 152, 70)); 
    head.setOutlineColor(sf::Color(85, 107, 47));
    head.setOrigin({ head.getRadius(), head.getRadius() });
    head.setPosition({ a, b - 2.f }); // Slightly above center

    // FIXED: Single cannon barrel with proper setup for rotation
    barrel.setSize({ 4.f, 18.f }); // Width, Height
    barrel.setFillColor(sf::Color(64, 64, 64, 255)); // Fully opaque dark gray
    barrel.setOutlineThickness(0.5f);
    barrel.setOutlineColor(sf::Color(32, 32, 32)); // Darker gray

    // CRITICAL FIX: Set origin to rotate around the back of the barrel
    // For a barrel that extends rightward when rotation = 0
    barrel.setOrigin({ 2.f, 2.f }); // Center horizontally, at the back vertically
    barrel.setPosition({ a, b - 2.f }); // Same as head position

    // Sight/targeting device (small rectangle on top of head)
    sight.setSize({ 2.f, 4.f });
    sight.setFillColor(sf::Color(139, 69, 19)); // Brown
    sight.setOrigin({ 1.f, 2.f }); // Center origin
    sight.setPosition({ a, b - 2.f });
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
    barrel.setRotation(a);
    sight.setRotation(a);
}
sf::Angle Tower::gettowerangle() {
    return head.getRotation();
}
void Tower::setfillcolordefault() {
    body.setFillColor(sf::Color(107, 142, 35,255));
    body.setOutlineColor(sf::Color(85, 107, 47,255));
    head.setFillColor(sf::Color(124, 152, 70,255));
    head.setOutlineColor(sf::Color(85, 107, 47,255));
    base.setFillColor(sf::Color(0x556b2fFF));
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
    barrel.setFillColor(sf::Color(64, 64, 64, 255));
    barrel.setOutlineColor(sf::Color(32, 32, 32, 255));
    sight.setFillColor(sf::Color(139, 69, 19,255));
}
void Tower::showrange() {
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}
void Tower::setfillcolorlight() {
    body.setFillColor(sf::Color(107, 142, 35,100));
    body.setOutlineColor(sf::Color(85, 107, 47,100));
    head.setFillColor(sf::Color(124, 152, 70,100));
    head.setOutlineColor(sf::Color(85, 107, 47,100));
    base.setFillColor(sf::Color(0x556b2f55));
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
    barrel.setFillColor(sf::Color(64, 64, 64, 100));
    barrel.setOutlineColor(sf::Color(32, 32, 32,100));
    sight.setFillColor(sf::Color(139, 69, 19,100));
}

void Tower::setfillcolorred() {
    body.setFillColor(sf::Color(200, 142, 35,100));
    body.setOutlineColor(sf::Color(200, 107, 47,100));
    head.setFillColor(sf::Color(200, 152, 70,100));
    head.setOutlineColor(sf::Color(200, 107, 47,100));
    base.setFillColor(sf::Color(0xFF6b2f55));
    Towerrange.setFillColor(sf::Color(0xFF000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
    barrel.setFillColor(sf::Color(200, 64, 64, 100));
    barrel.setOutlineColor(sf::Color(200, 32, 32,100));
    sight.setFillColor(sf::Color(200, 69, 19,100));
}


sf::Vector2f Tower::gettowerposition() {
    return body.getPosition();
}

float Tower::getradius() {
    return head.getRadius();
}

void Tower::setposition(sf::Vector2f position) {
    body.setPosition(position);
    head.setPosition(body.getPosition());
    Towerrange.setPosition(body.getPosition());
    base.setPosition(body.getPosition());
    barrel.setPosition(body.getPosition());
    sight.setPosition(body.getPosition());
}

void Tower::draw(sf::RenderWindow& window) const {
    for (const auto& bullet : bullets)
        bullet.draw(window);
    window.draw(base);
    window.draw(body);
    window.draw(head);
    window.draw(Towerrange);
    window.draw(barrel);
    window.draw(sight);
}

bool Tower::contain(sf::Vector2f mousepos) {
    return head.getGlobalBounds().contains(mousepos) || body.getGlobalBounds().contains(mousepos);
}

float Tower::getrange() {
    return Towerrange.getRadius();
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

void Tower::tryShoot(std::vector<Enemy*>& enemies) {
    if (Tower::fireCooldown.getElapsedTime().asSeconds() >= fireRate) {
        for (auto& enemy : enemies) {
            if (isInRange(enemy->getposition(),50.f)) {
                bullets.emplace_back(body.getPosition(), enemy->getposition());
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

