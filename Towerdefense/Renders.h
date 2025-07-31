#pragma once
#include <SFML/Graphics.hpp>
#include "Towers.h"
#include "GameConstants.h"
#include "enemy.h"
#include"gamesession.h"
class Shop {
public:
    Shop();
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, PlayerInfo* info);
    void update(const sf::RenderWindow& window, float dt);
    bool bounded();
    void Towertarget(std::vector<Enemy*>& enemies, float dt);
    void Towerupgradeshop(PlayerInfo* info);
private:
    std::vector<sf::RectangleShape> frames;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Tower>> deployedtowers;
    Tower* operatedtower = nullptr;
    sf::RectangleShape bar;
    bool dragging = false;
    std::unique_ptr<Tower> draggedTower;
    std::unique_ptr<Tower> shopTower;
    bool clicked = false;
    sf::RectangleShape shopbar;
    sf::Text shopTowername;
    sf::Font font;
};

class aim {

};