#pragma once
#include <SFML/Graphics.hpp>
#include "Towers.h"
#include "GameConstants.h"

class Shop {
public:
    Shop();
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);

private:
    std::vector<sf::RectangleShape> frames;
    std::vector<Tower> towers;
    std::vector<Tower> deployedtowers;
    sf::RectangleShape bar;
    bool dragging = false;
    Tower draggedTower;
};
