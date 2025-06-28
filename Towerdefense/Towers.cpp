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
    Towerrange.setRadius(50.f);
    Towerrange.setOrigin(Towerrange.getGeometricCenter());
    Towerrange.setOutlineColor(sf::Color(0x80808000));
    Towerrange.setOutlineThickness(1.f);
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setPosition(body.getPosition());
}

void Tower::setfillcolordefault() {
    body.setFillColor(sf::Color(0x6464C8FF));
    head.setFillColor(sf::Color(0x0000FFFF));
    Towerrange.setFillColor(sf::Color(0x00000000));
    Towerrange.setOutlineColor(sf::Color(0x80808000));
}
void Tower::showrange() {
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}
void Tower::setfillcolorlight() {
    body.setFillColor(sf::Color(0x6464C855));
    head.setFillColor(sf::Color(0x0000FF55));
    Towerrange.setFillColor(sf::Color(0x00000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}
sf::FloatRect Tower::getGlobalBounds(){
    return head.getGlobalBounds();
}

void Tower::setfillcolorred() {
    body.setFillColor(sf::Color(0xFF64C855));
    head.setFillColor(sf::Color(0xFF00FF55));
    Towerrange.setFillColor(sf::Color(0xFF000022));
    Towerrange.setOutlineColor(sf::Color(0x80808088));
}


sf::Vector2f Tower::towerpos() {
    return body.getPosition();
}


void Tower::setposition(sf::Vector2f position) {
    body.setPosition(position);
    head.setPosition(body.getPosition());
    Towerrange.setPosition(body.getPosition());
}

void Tower::draw(sf::RenderWindow& window) const {
    window.draw(body);
    window.draw(head);
    window.draw(Towerrange);
}

bool Tower::contain(sf::Vector2f mousepos) {
    return head.getGlobalBounds().contains(mousepos) || body.getGlobalBounds().contains(mousepos);
}

