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
    // Soldier sprite components
    sf::CircleShape helmet;          // Soldier helmet
    sf::CircleShape leftGoggle;      // Left goggle
    sf::CircleShape rightGoggle;     // Right goggle
    sf::RectangleShape body;         // Main torso
    sf::RectangleShape chestArmor;   // Chest armor/vest
    sf::RectangleShape leftArm;      // Left arm
    sf::RectangleShape rightArm;     // Right arm
    sf::RectangleShape leftLeg;      // Left leg
    sf::RectangleShape rightLeg;     // Right leg
    sf::RectangleShape weapon;       // Rifle/weapon

    // Pathfinding and movement
    std::vector<sf::Vector2i> path;
    std::vector<std::vector<bool>> visited;
    size_t currentStep;

    // Health system
    float maxHealth;
    float currentHealth;

    // Health bar components
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;

    // Helper methods
    void initializeSoldierSprite();
    void setSoldierPosition(sf::Vector2f position);
    sf::Vector2f getSoldierPosition() const;
    void updateHealthBar();
    void findPath(int x, int y);

    // Static directions for pathfinding
    static const std::vector<sf::Vector2i> directions;
};