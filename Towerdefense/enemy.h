#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>
#include "GameConstants.h" 

class Enemy {
public:
    Enemy();
    float speed = 0.8f;
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds();
    sf::Vector2f getposition();
    // for the body parts of the black troop
    sf::CircleShape hood;
    sf::CircleShape faceMask;
    sf::CircleShape body;
    sf::RectangleShape leftArm;
    sf::RectangleShape rightArm;
    sf::CircleShape leftHand;
    sf::CircleShape rightHand;
    sf::RectangleShape leftLeg;
    sf::RectangleShape rightLeg;
    sf::RectangleShape leftFoot;
    sf::RectangleShape rightFoot;
    sf::RectangleShape belt;


    sf::CircleShape leftEye;
    sf::CircleShape rightEye;
    sf::CircleShape leftEyeGlow;
    sf::CircleShape rightEyeGlow;


    sf::RectangleShape swordBlade;
    sf::RectangleShape swordGuard;
    sf::RectangleShape swordHandle;
    sf::CircleShape swordPommel;
    sf::RectangleShape healthbar;

    std::vector<sf::Vector2i> path;
    bool visited[Game::MAP_HEIGHT][Game::MAP_WIDTH];
    size_t currentStep = 0;
    sf::Vector2f position;


    float movementTimer;
    bool isMoving;

    const std::vector<sf::Vector2i> directions = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}
    };


    void findPath(int x, int y);
    void updateNinjaPosition(sf::Vector2f newPos);
    bool isAlive = true;
    float Health = 100.0f;
    float maxHealth = 100.f;
    void updateHealthbarnev();
};


class FastEnemy : public Enemy {
public:
    FastEnemy();
    void update(float deltaTime);
};