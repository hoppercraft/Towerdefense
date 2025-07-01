#include "Towers.h"
#include "Bullet.h"
#include"iostream"

Tower::Tower(float a, float b) {
    base.setRadius(12.f);
    base.setPointCount(8); // Octagonal base
    base.setFillColor(sf::Color(85, 107, 47)); // Dark olive green
    base.setOutlineThickness(1.f);
    base.setOutlineColor(sf::Color(60, 75, 35)); // Darker green outline
    base.setOrigin({ base.getRadius(), base.getRadius() });
    base.setPosition({ a, b });

    // Main turret body (larger rectangular body)
    body.setSize({ 16.f, 20.f });
    body.setFillColor(sf::Color(107, 142, 35)); // Olive drab
    body.setOutlineThickness(1.f);
    body.setOutlineColor(sf::Color(85, 107, 47)); // Darker outline
    body.setOrigin({ 8.f, 10.f }); // Center of rectangle
    body.setPosition({ a, b });

    // Turret head/cabin (rounded top section) - This will be the rotation center
    head.setRadius(8.f);
    head.setFillColor(sf::Color(124, 152, 70)); // Lighter olive
    head.setOutlineThickness(1.f);
    head.setOutlineColor(sf::Color(85, 107, 47));
    head.setOrigin({ head.getRadius(), head.getRadius() });
    head.setPosition({ a, b - 2.f }); // Slightly above center

    // FIXED: Single cannon barrel with proper setup for rotation
    sf::RectangleShape barrel;
    barrel.setSize({ 4.f, 18.f }); // Width, Height
    barrel.setFillColor(sf::Color(64, 64, 64, 255)); // Fully opaque dark gray
    barrel.setOutlineThickness(0.5f);
    barrel.setOutlineColor(sf::Color(32, 32, 32)); // Darker gray

    // CRITICAL FIX: Set origin to rotate around the back of the barrel
    // For a barrel that extends rightward when rotation = 0
    barrel.setOrigin({ 2.f, 2.f }); // Center horizontally, at the back vertically
    barrel.setPosition({ a, b - 2.f }); // Same as head position

    barrels.push_back(barrel);

    // Single muzzle flash indicator
    sf::CircleShape muzzle;
    muzzle.setRadius(2.f);
    muzzle.setFillColor(sf::Color(255, 140, 0, 180)); // Orange with some transparency
    muzzle.setOrigin({ muzzle.getRadius(), muzzle.getRadius() });
    muzzle.setPosition({ a, b - 2.f });


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
    window.draw(base);
    window.draw(body);
    window.draw(head);
    window.draw(sight);
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

