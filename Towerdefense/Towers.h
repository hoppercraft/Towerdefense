#pragma once
#include <SFML/Graphics.hpp>

class Tower {
public:
    Tower(float a = -10.f, float b = 10.f);

    void setfillcolordefault();
    void setfillcolorlight();
    void setfillcolorred();
    void setposition(sf::Vector2f position);
    void draw(sf::RenderWindow& window) const;
    bool contain(sf::Vector2f mousepos);

private:
    sf::RectangleShape body;
    sf::CircleShape head;
};

