
#pragma once
#include <SFML/Graphics.hpp>

class PlayerInfo {
public:
    PlayerInfo(int coin = 500, int health = 5);
    void draw(sf::RenderWindow& window);
    void turrentplaced(int cost);
    void enemypassed(int healthlost);
    bool enoughmoney(int cost);
    void notEnoughMoneywarning();
    void coinsearned(int m);
    bool gameover();
private:
    sf::Font font;
    sf::Text cointext;
    sf::Text hearttext;
    sf::Text warningtext;
    sf::CircleShape coinShape;
    sf::ConvexShape heartShape;
    int coins;
    int health;
    int i;
    sf::Clock timer;
    sf::Time offset;
    sf::Time elapsed;
};
