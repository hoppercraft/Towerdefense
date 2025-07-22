#pragma once
#include <SFML/Graphics.hpp>
#include "Towers.h"
#include "GameConstants.h"
#include <memory>
#include <vector>

// Forward declarations
class Enemy;

enum class ColorState {
    DEFAULT,
    LIGHT,
    RED
};

class Shop {
public:
    Shop();
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window, float dt);
    bool bounded();
    void Towertarget(std::vector<Enemy*>& enemies, float dt);

private:
    std::vector<sf::RectangleShape> frames;
    std::vector<std::unique_ptr<Tower>> shopTowers; // Shop display towers
    std::vector<std::unique_ptr<Tower>> deployedtowers; // Deployed towers
    Tower* operatedtower = nullptr;
    sf::RectangleShape bar;
    bool dragging = false;
    std::unique_ptr<Tower> draggedTower;
    bool clicked = false;

    // Helper functions
    std::unique_ptr<Tower> createTower(TowerType type, float x, float y);
    void setTowerColor(Tower* tower, ColorState state);
};

class Aim {
    // Placeholder for aim functionality
};