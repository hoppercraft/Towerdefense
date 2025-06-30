#pragma once
#include <SFML/Graphics.hpp>
#include "Towers.h"
#include "GameConstants.h"
#include "enemy.h"
class Shop {
public:
    Shop();
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    bool bounded();
    void Towertarget(std::vector<Enemy>& enemies,float dt);

private:
    std::vector<sf::RectangleShape> frames;
    std::vector<Tower> towers;
    std::vector<Tower> deployedtowers;
    Tower* operatedtower=nullptr;
    sf::RectangleShape bar;
    bool dragging = false;
    Tower draggedTower;
    bool clicked = false;
};

class aim {

};