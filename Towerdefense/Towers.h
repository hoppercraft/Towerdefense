#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
// Forward declarations to avoid circular dependencies
class Enemy;
class Bullet;
enum class TowerType {
    BASIC,
    TESLA
};
class Tower {
public:
    Tower(float a = 0, float b = 0, TowerType type = TowerType::BASIC);
    virtual ~Tower() = default;
    // Color management functions
    virtual void setfillcolordefault();
    virtual void setfillcolorlight();
    virtual void setfillcolorred();
    // Position and drawing functions
    sf::Vector2f gettowerposition();
    virtual void setposition(sf::Vector2f position);
    virtual void draw(sf::RenderWindow& window) const;
    virtual bool contain(sf::Vector2f mousepos);
    void showrange();
    // Rotation functions
    virtual void setangle(float x);  // Made virtual so Tesla can override
    sf::Angle gettowerangle();
    // Combat functions
    virtual void tryShoot(std::vector<Enemy*>& enemies);
    void updateBullets(float dt);
    bool isInRange(sf::Vector2f other, float range);
    // Getters
    float getrange();
    float getradius();
    TowerType getTowerType() const { return towerType; }
protected:
    TowerType towerType;
    sf::CircleShape Towerrange;
    std::vector<Bullet> bullets;
    sf::Clock fireCooldown;
    float fireRate = 0.5f;
    float range = 50.0f; // Fixed: was 100.0f, changed to match usage in code
    sf::Vector2f position;
    float angle = 0.0f;
    // Basic tower components (used by BasicTower)
    sf::CircleShape base;
    sf::RectangleShape body;
    sf::CircleShape head;
    sf::RectangleShape sight;
    sf::RectangleShape barrel;
    virtual void initializeComponents();
};
class BasicTower : public Tower {
public:
    BasicTower(float a = 0, float b = 0);
    void draw(sf::RenderWindow& window) const override;
private:
    void initializeComponents() override;
};
class TeslaTower : public Tower {
public:
    TeslaTower(float a = 0, float b = 0);

    void draw(sf::RenderWindow& window) const override;
    void tryShoot(std::vector<Enemy*>& enemies) override;
    void setposition(sf::Vector2f pos) override;
    void setangle(float x) override;  // Override to prevent rotation of base Tower components
    bool contain(sf::Vector2f mousepos) override;
    // Tesla-specific color functions
    void setfillcolordefault() override;
    void setfillcolorlight() override;
    void setfillcolorred() override;
    void updateLightning();
private:
    void initializeComponents() override;
    // Tesla tower specific components
    sf::CircleShape teslaBase;
    sf::CircleShape teslaCoil1;
    sf::CircleShape teslaCoil2;
    sf::CircleShape teslaCoil3;
    sf::CircleShape teslaTop;
    sf::RectangleShape teslaRod;
    std::vector<sf::RectangleShape> lightningArcs;
    sf::Clock lightningClock;
};