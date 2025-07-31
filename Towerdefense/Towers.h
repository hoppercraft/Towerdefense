#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"
#include"Bullet.h"
#include"GameConstants.h"
class Tower {
public:
    Tower(float a = 0, float b = 0);
    virtual void setfillcolordefault();
    virtual void setfillcolorlight();
    virtual void setfillcolorred();
    virtual void setposition(sf::Vector2f position);
    virtual void draw(sf::RenderWindow& window) const;
    virtual bool contain(sf::Vector2f mousepos);
    virtual void showrange();
    virtual void hiderange();
    virtual void setangle(float x);
    virtual sf::Angle gettowerangle();
    virtual void tryShoot(std::vector<Enemy*>& enemies);
    void updateBullets(float dt);
    virtual bool isInRange(sf::Vector2f other, float range);
    virtual float getrange();
    virtual float getradius();
    virtual sf::Vector2f gettowerposition();
    std::vector<Bullet> bullets;
    sf::Clock fireCooldown;
    float fireRate = 1.f;
    virtual std::unique_ptr<Tower> clone() const;
    virtual ~Tower() = default;
    virtual Game::TileType towertilereq();
    void clearBullets();
    virtual std::string gettowername();
    bool upgraded=false;
    virtual void upgrade();
    int towercost;
protected:
    sf::CircleShape Towerrange;
    sf::CircleShape base;
    sf::RectangleShape body;
    sf::CircleShape head;
    sf::RectangleShape sight;
    std::vector<sf::RectangleShape> barrels;
    sf::RectangleShape barrel;
};

class CannonTower : public Tower {
public:
    CannonTower(float x = 0, float y = 0);

    std::unique_ptr<Tower> clone() const override;
};


class Boat : public Tower {
public:
    Boat(float x = 100, float y = 100);
    void draw(sf::RenderWindow& window) const override;
    void setfillcolordefault() override;
    void setfillcolorlight() override;
    void setfillcolorred() override;
    void setposition(sf::Vector2f position) override;
    bool contain(sf::Vector2f mousepos) override;
    void showrange() override;
    void setangle(float x) override;
    sf::Angle gettowerangle() override;
    bool isInRange(sf::Vector2f other, float range) override;
    float getrange() override;
    float getradius() override;
    sf::Vector2f gettowerposition() override;
    std::unique_ptr<Tower> clone() const override;
    void tryShoot(std::vector<Enemy*>& enemies) override;
    Game::TileType towertilereq() override;
    std::string gettowername() override;
    void upgrade() override;
    void hiderange() override;
private:
    sf::CircleShape Towerrange;
    sf::ConvexShape boat;
    sf::RectangleShape flag;
    sf::RectangleShape cannonL,cannonR;
    sf::CircleShape skull,man;
    sf::RectangleShape crossbone1, crossbone2;
};
