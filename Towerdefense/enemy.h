#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>
#include "GameConstants.h" 

class Enemy {
public:
    Enemy();
    void update(float deltaTime, float speed=1);
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
};


class FastEnemy : public Enemy {
public:
    FastEnemy();
    void update(float deltaTime);
};  
