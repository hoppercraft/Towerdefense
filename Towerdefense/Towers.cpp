#include "Towers.h"

Tower::Tower(float a, float b) {
    body.setSize({ 10.f, 15.f });
    body.setFillColor(sf::Color(0x6464C8FF));
    body.setOrigin({ body.getGeometricCenter().x, head.getRadius() });
    body.setPosition({ a, b });

    head.setRadius(7.f);
    head.setFillColor(sf::Color(0x0000FFFF));
    head.setOrigin(head.getGeometricCenter());
    head.setPosition(body.getPosition());
}

void Tower::setfillcolordefault() {
    body.setFillColor(sf::Color(0x6464C8FF));
    head.setFillColor(sf::Color(0x0000FFFF));
}

void Tower::setfillcolorlight() {
    body.setFillColor(sf::Color(0x6464C855));
    head.setFillColor(sf::Color(0x0000FF55));
}

void Tower::setfillcolorred() {
    body.setFillColor(sf::Color(0xFF64C855));
    head.setFillColor(sf::Color(0xFF00FF55));
}

void Tower::setposition(sf::Vector2f position) {
    body.setPosition(position);
    head.setPosition(body.getPosition());
}

void Tower::draw(sf::RenderWindow& window) const {
    window.draw(body);
    window.draw(head);
}

bool Tower::contain(sf::Vector2f mousepos) {
    return head.getGlobalBounds().contains(mousepos) || body.getGlobalBounds().contains(mousepos);
}
