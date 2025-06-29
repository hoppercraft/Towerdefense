#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameConstants.h" 

class Enemy {
public:
    Enemy();

    void update(float speed);
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
    void takeDamage(float damage);
    bool isAlive() const;
    float getHealth() const;
    float getMaxHealth() const;

private:
    sf::CircleShape shape;
    std::vector<sf::Vector2i> path;
    std::vector<std::vector<bool>> visited;
    size_t currentStep;

    // Health system
    float maxHealth;
    float currentHealth;

    // Health bar components
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;

    void updateHealthBar();
    void findPath(int x, int y);

    // Static directions - this is the key fix
    static const std::vector<sf::Vector2i> directions;
};