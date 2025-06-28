#pragma once
#include <SFML/Graphics.hpp>
class Tower {
public:
    Tower(float a = 0, float b = 0);

    void setfillcolordefault();
    void setfillcolorlight();
    void setfillcolorred();
    sf::Vector2f towerpos();
    void setposition(sf::Vector2f position);
    void draw(sf::RenderWindow& window) const;
    bool contain(sf::Vector2f mousepos);
    void showrange();
    sf::FloatRect getGlobalBounds();
protected:
    sf::RectangleShape body;
    sf::CircleShape head;
    sf::CircleShape Towerrange;
};

