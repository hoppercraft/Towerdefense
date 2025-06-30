#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "GameConstants.h" 

class Enemy {
public:
    Enemy();
    void update(float speed);
    void draw(sf::RenderWindow& window); 
    sf::FloatRect getGlobalBounds();
    sf::Vector2f getposition();
private:
    sf::CircleShape shape;
    std::vector<sf::Vector2i> path;
    bool visited[Game::MAP_HEIGHT][Game::MAP_WIDTH];
    size_t currentStep = 0;

    const std::vector<sf::Vector2i> directions = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}
    };

    void findPath(int x, int y);
};
