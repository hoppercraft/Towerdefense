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

    const std::vector<std::unique_ptr<Tower>>& getDeployedTowers() const {
        return deployedtowers;
    } 

    void clearAllTowers() {
        deployedtowers.clear();
        if (operatedtower) {
            operatedtower = nullptr;
            shopTower = nullptr;
            clicked = false;
        }
    }

    void addSavedTower(std::unique_ptr<Tower> tower) {
        deployedtowers.push_back(std::move(tower));
    }

private:
    std::vector<sf::RectangleShape> frames;
    sf::Text Cost1;
    sf::Text Cost2;
    sf::Text Cost3;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Tower>> deployedtowers;
    Tower* operatedtower = nullptr;
    sf::RectangleShape bar;
    bool dragging = false;
    std::unique_ptr<Tower> draggedTower;
    std::unique_ptr<Tower> shopTower;
    bool clicked = false;
    sf::RectangleShape shopbar,cross1,cross2;
    sf::Text shopTowername,upgrade,sell;
    sf::Font font;
    sf::RectangleShape upgradeshopframe,sellframe,hammer,pole;
};

class aim {

};