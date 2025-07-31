#include "Towers.h"
#include "Bullet.h"
#include"iostream"
#include "GameConstants.h"
Tower::Tower(float a, float b) {
    towercost=100;
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

void Tower::hiderange() {
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
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
    if(upgraded){
    }
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
            if (isInRange(enemy->getposition(),Towerrange.getRadius())) {
                bullets.emplace_back(body.getPosition(), enemy);
                fireCooldown.restart();
                if (upgraded) {
                    bullets.back().increasebulletdamage();
                }
                break;
            }
        }
    }
}
std::string Tower::gettowername() {
    return("Cannon");
}

void Tower::updateBullets(float dt) {
    for (auto& bullet : bullets) {
        bullet.update(dt);
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return b.reachedTarget() || !b.isMoving(); }), bullets.end());
}

void Tower::clearBullets() {
    bullets.clear();
}

void Tower::upgrade() {
    fireRate = 0.5f;
    Towerrange.setRadius(80.f);
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    upgraded = true;
    body.setFillColor(sf::Color(100, 132, 30, 255));
    body.setOutlineColor(sf::Color(85, 107, 47, 255));
    head.setFillColor(sf::Color(114, 142, 60, 255));
    head.setOutlineColor(sf::Color(85, 107, 47, 255));
    base.setFillColor(sf::Color(0x505b1fFF));
    barrel.setFillColor(sf::Color(64, 64, 64, 255));
    barrel.setOutlineColor(sf::Color(32, 32, 32, 255));
    sight.setFillColor(sf::Color(187, 187, 187, 255));
}

std::unique_ptr<Tower> Tower::clone() const {
    return std::make_unique<Tower>(*this);
}

Game::TileType Tower::towertilereq() {
    return (Game::TileType::Grass);
}

CannonTower::CannonTower(float x, float y) : Tower(x, y) {
    Towerrange.setRadius(60.f);     
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    Towerrange.setPosition(body.getPosition());
}

std::unique_ptr<Tower> CannonTower::clone() const {
    return std::make_unique<CannonTower>(*this);
}

Boat::Boat(float x, float y) {
    towercost=150;
    boat.setFillColor(sf::Color(133, 89, 54));
    boat.setPosition({ x,y });
    boat.setOutlineColor(sf::Color(212, 143, 87));
    boat.setPointCount(5);
    boat.setScale({0.4f, 0.4f});
    boat.setPoint(0, sf::Vector2f(17, 20));
    boat.setPoint(1, sf::Vector2f(27, 11));
    boat.setPoint(2, sf::Vector2f(60, 11));
    boat.setPoint(3, sf::Vector2f(60, 29));
    boat.setPoint(4, sf::Vector2f(27, 29));
    boat.setOutlineThickness(5.f);
    boat.setOrigin(boat.getGeometricCenter());
    flag.setSize({8.0f, 14.0f});
    flag.setFillColor(sf::Color(232, 228, 201));
    flag.setOrigin({ flag.getGeometricCenter().x-4,flag.getGeometricCenter().y });
    skull.setRadius(2.0f);
    skull.setScale({ 0.5f,1.f });
    skull.setFillColor(sf::Color::White);
    skull.setOrigin({ skull.getGeometricCenter().x-13,skull.getGeometricCenter().y });
    crossbone1.setSize({8.0f, 1.0f});
    crossbone1.setFillColor(sf::Color::White);
    crossbone1.setScale({ 1.f,0.5f });
    crossbone1.setOrigin({ crossbone1.getGeometricCenter().x-2,crossbone1.getGeometricCenter().y+8});
    crossbone2.setSize({8.0f, 1.0f});
    crossbone2.setFillColor(sf::Color::White);
    crossbone2.setScale({ 1.f,0.5f });
    crossbone1.setRotation(sf::degrees(60.f));
    crossbone2.setRotation(sf::degrees(120.f));
    crossbone2.setOrigin({ crossbone2.getGeometricCenter().x+2,crossbone2.getGeometricCenter().y+8 });
    flag.setPosition({x,y});
    skull.setPosition({ x,y });
    // Position crossbones in X formation
    crossbone1.setPosition({ x,y });
    crossbone2.setPosition({ x,y });
    cannonL.setSize({ 4.f,10.f });
    cannonL.setFillColor(sf::Color(48, 48, 47));
    cannonL.setOrigin({ cannonL.getGeometricCenter().x,cannonL.getGeometricCenter().y-5 });
    cannonL.setPosition({ x,y });    
    cannonR.setSize({ 4.f,10.f });
    cannonR.setFillColor(sf::Color(48, 48, 47));
    cannonR.setOrigin({ cannonL.getGeometricCenter().x,cannonL.getGeometricCenter().y + 5 });
    cannonR.setPosition({ x,y });
    Towerrange.setOrigin({ Towerrange.getGeometricCenter().x+75,Towerrange.getGeometricCenter().y + 75 });
    Towerrange.setRadius(75.f);
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
    man.setFillColor(sf::Color(164, 91, 72));
    man.setRadius(3.f);
    man.setOrigin({ man.getGeometricCenter().x-4,man.getGeometricCenter().y });
    man.setPosition({ x,y });
}

void Boat::draw(sf::RenderWindow& window) const {
    for (const auto& bullet : bullets)
        bullet.draw(window);
    window.draw(cannonL);
    window.draw(cannonR);
    window.draw(boat);
    window.draw(Towerrange);
    window.draw(flag);
    if (!upgraded) {
        window.draw(man);
    }
    if (upgraded) {
        window.draw(skull);
        window.draw(crossbone1);
        window.draw(crossbone2);
    }
}

std::string Boat::gettowername() {
    return("Boat");
}

void Boat::setangle(float x) {
    sf::Angle a = sf::radians(x);
    boat.setRotation(a);
    flag.setRotation(a);
    skull.setRotation(a);
    cannonL.setRotation(a);
    cannonR.setRotation(a);
    crossbone1.setRotation(a+ sf::degrees(60.f));
    crossbone2.setRotation(a+ sf::degrees(120.f));
    man.setRotation(a);
}

void Boat::setfillcolordefault() {
    boat.setFillColor(sf::Color(133, 89, 54,255));
    boat.setOutlineColor(sf::Color(212, 143, 87,255));
    flag.setFillColor(sf::Color(232, 228, 201,255));
    skull.setFillColor(sf::Color(255,255,255,255));
    crossbone1.setFillColor(sf::Color(255, 255, 255, 255));
    crossbone2.setFillColor(sf::Color(255, 255, 255, 255));
    cannonL.setFillColor(sf::Color(48, 48, 47,255));
    cannonR.setFillColor(sf::Color(48, 48, 47,255));
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
    man.setFillColor(sf::Color(164, 91, 72));
}

sf::Angle Boat::gettowerangle() {
    return boat.getRotation();
}

void Boat::showrange() {
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}

void Boat::hiderange() {
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
}

void Boat::setfillcolorlight() {
    boat.setFillColor(sf::Color(133, 89, 54, 100));
    boat.setOutlineColor(sf::Color(212, 143, 87, 100));
    flag.setFillColor(sf::Color(232, 228, 201, 100));
    skull.setFillColor(sf::Color(255, 255, 255, 100));
    crossbone1.setFillColor(sf::Color(255, 255, 255, 100));
    crossbone2.setFillColor(sf::Color(255, 255, 255, 100));
    cannonL.setFillColor(sf::Color(48, 48, 47, 100));
    cannonR.setFillColor(sf::Color(48, 48, 47, 100));
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
    man.setFillColor(sf::Color(164, 91, 72,100));
}

void Boat::setfillcolorred() {
    boat.setFillColor(sf::Color(200, 89, 54, 100));
    boat.setOutlineColor(sf::Color(255, 143, 87, 100));
    flag.setFillColor(sf::Color(255, 200, 201, 100));
    skull.setFillColor(sf::Color(255, 200, 200, 100));
    crossbone1.setFillColor(sf::Color(255, 200, 200, 100));
    crossbone2.setFillColor(sf::Color(255, 200, 200, 100));
    cannonL.setFillColor(sf::Color(200, 48, 47, 100));
    cannonR.setFillColor(sf::Color(200, 48, 47, 100));
    Towerrange.setFillColor(sf::Color(0xFF000022));
    Towerrange.setOutlineColor(sf::Color(0xFF808088));
    man.setFillColor(sf::Color(255, 91, 72,100));
}

sf::Vector2f Boat::gettowerposition() {
    return boat.getPosition();
}

float Boat::getradius() {
    return (5.f);
}

void Boat::setposition(sf::Vector2f position) {
    boat.setPosition(position);
    flag.setPosition(position);
    Towerrange.setPosition(position);
    skull.setPosition(position);
    crossbone1.setPosition(position);
    crossbone2.setPosition(position);
    cannonL.setPosition(position);
    cannonR.setPosition(position);
    man.setPosition({ position.x,position.y });
}

bool Boat::contain(sf::Vector2f mousepos) {
    return boat.getGlobalBounds().contains(mousepos);
}

float Boat::getrange() {
    return Towerrange.getRadius();
}

bool Boat::isInRange(sf::Vector2f other, float range) {
    sf::Vector2f center = boat.getPosition();
    float dx = center.x - other.x;
    float dy = center.y - other.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance <= range) {
        return true;
    }
    else
        return false;
}

std::unique_ptr<Tower> Boat::clone() const {
    return std::make_unique<Boat>(*this);
}

void Boat::tryShoot(std::vector<Enemy*>& enemies) {
    if (Tower::fireCooldown.getElapsedTime().asSeconds() >= fireRate) {
        for (auto& enemy : enemies) {
            if (isInRange(enemy->getposition(), Towerrange.getRadius())) {
                bullets.emplace_back(boat.getPosition(), enemy);
                fireCooldown.restart();
                if (upgraded) {
                    bullets.back().increasebulletdamage();
                }
                break;
            }
        }
    }
}

Game::TileType Boat::towertilereq() {
    return (Game::TileType::Water);
}

void Boat::upgrade() {
    fireRate = 0.5f;
    Towerrange.setRadius(90.f);
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    flag.setFillColor(sf::Color(00, 00, 00, 255));
    upgraded = true;
    boat.setPoint(0, sf::Vector2f(10, 20));
    boat.setPoint(1, sf::Vector2f(20, 10));
    boat.setPoint(2, sf::Vector2f(60, 10));
    boat.setPoint(3, sf::Vector2f(60, 30));
    boat.setPoint(4, sf::Vector2f(20, 30));
    flag.setSize({ 8.0f, 16.0f });
    flag.setOrigin({ flag.getGeometricCenter().x - 4, flag.getGeometricCenter().y });
    cannonL.setSize({ 5.f,10.f });
    cannonR.setSize({ 5.f,10.f });
}