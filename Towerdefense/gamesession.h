#pragma once
#include <SFML/Graphics.hpp>

class PlayerInfo {
public:
    PlayerInfo(int coin = 500, int health = 100);
    void draw(sf::RenderWindow& window);
private:
    sf::Font font;
    sf::Text cointext;
    sf::Text hearttext;
    sf::CircleShape coinShape;
    sf::ConvexShape heartShape;
    int coins;
    int health;
};