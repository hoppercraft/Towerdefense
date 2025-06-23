#pragma once
#include <SFML/Graphics.hpp>  // SFML 3.0 unified header

class Enemy {
public:
    Enemy();
    void update(float speed);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

private:
    void findPath(int x, int y);

    sf::CircleShape shape;
    std::vector<sf::Vector2i> path;
    bool visited[8][13] = { {false} };  // Zero-initialized
    size_t currentStep = 0;

    static constexpr std::array<sf::Vector2i, 4> directions = {
        sf::Vector2i{1, 0},  // Right
        sf::Vector2i{0, 1},   // Down
        sf::Vector2i{-1, 0},  // Left
        sf::Vector2i{0, -1}   // Up
    };
};