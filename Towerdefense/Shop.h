#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Shop
{
public:
    Shop();
    void draw(sf::RenderWindow& window) const;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    const std::vector<sf::CircleShape>& getDeployedTowers() const;

private:
    std::vector<sf::RectangleShape> frames;
    std::vector<sf::CircleShape> towers;
    std::vector<sf::CircleShape> deployedTowers;
    sf::RectangleShape bar;
    bool dragging = false;
    sf::CircleShape draggedTower;
};