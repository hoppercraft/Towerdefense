#include "Renders.h"
#include"Bullet.h"
#include <iostream>
#include <cmath>
Shop::Shop() {
    bar.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE, 0 });
    bar.setSize({ Game::TILE_SIZE * 3, Game::TILE_SIZE * Game::MAP_HEIGHT });
    bar.setFillColor(sf::Color(0x7C5123FF));
    bar.setOutlineThickness({ 2.0f });
    bar.setOutlineColor(sf::Color(0xa0acaaFF));

    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape frame;
        frame.setSize({ Game::TILE_SIZE * 1.75f, Game::TILE_SIZE * 1.75f });
        frame.setOrigin(frame.getGeometricCenter());
        frame.setPosition({
            Game::MAP_WIDTH * Game::TILE_SIZE + bar.getGeometricCenter().x,
            Game::TILE_SIZE * 1.25f + i * Game::TILE_SIZE * 2.0f
            });
        frame.setFillColor(sf::Color(0xC4B093FF));
        frames.push_back(frame);

        Tower tower(frame.getPosition().x, frame.getPosition().y);
        towers.push_back(tower);
    }
}

void Shop::draw(sf::RenderWindow& window) {
    window.draw(bar);
    for (const auto& frame : frames)
        window.draw(frame);
    for (const auto& tower : towers)
        tower.draw(window);
    if (dragging)
        draggedTower.draw(window);
    for (const auto& tower : deployedtowers)
        tower.draw(window);
    if (clicked) {
        operatedtower->draw(window);
    }
}


void Shop::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f worldPos = mousePos;
    int tileX = static_cast<int>(worldPos.x) / Game::TILE_SIZE;
    int tileY = static_cast<int>(worldPos.y) / Game::TILE_SIZE;
    if (const auto* mousepressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousepressed->button == sf::Mouse::Button::Left) {
            if (clicked == true) {
                operatedtower->setfillcolordefault();
                clicked = false;
            }
            for (size_t i = 0; i < deployedtowers.size(); ++i) {
                if (deployedtowers[i].contain(mousePos)) {
                    operatedtower = &deployedtowers[i];
                    operatedtower->showrange();
                    clicked = true;
                    break;
                }
            }
            for (size_t i = 0; i < towers.size(); ++i) {
                if (towers[i].contain(mousePos)) {
                    dragging = true;
                    draggedTower = towers[i];
                    draggedTower.setfillcolorlight();
                    draggedTower.setposition(mousePos);
                    break;
                }
            }
        }
    }
    else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left && dragging) {
            dragging = false;
            if (tileX >= 0 && tileX < Game::MAP_WIDTH && tileY >= 0 && tileY < Game::MAP_HEIGHT) {
                if (Game::Map1[tileY][tileX] == Game::TileType::Grass) {
                    if (Shop::bounded() == false) {
                        draggedTower.setfillcolordefault();
                        deployedtowers.push_back(draggedTower);
                    }
                }
            }
        }
    }



    if (tileX >= 0 && tileX < Game::MAP_WIDTH && tileY >= 0 && tileY < Game::MAP_HEIGHT) {
        if (Game::Map1[tileY][tileX] == Game::TileType::Grass && Shop::bounded()==false) {
            draggedTower.setfillcolorlight();
        }
        else {
            draggedTower.setfillcolorred();
        }
    }
}

bool Shop::bounded() {
    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        sf::Vector2f center = deployedtowers[i].gettowerposition();
        sf::Vector2f target = draggedTower.gettowerposition();
        float dx = center.x - target.x;
        float dy = target.y - center.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance<= 14.0f){
            return true;
        }
    }
    return false;
}
void Shop::update(const sf::RenderWindow& window) {
    if (dragging) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        draggedTower.setposition(mousePos);
    }
}

void Shop::Towertarget(std::vector<Enemy>& enemies,float dt){
    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        for (size_t j = 0; j < enemies.size(); ++j) {
            sf::Vector2f center = deployedtowers[i].gettowerposition();
            sf::Vector2f target = enemies[j].getposition();
            float dx = center.x - target.x;
            float dy = target.y - center.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (deployedtowers[i].isInRange(enemies[j].getposition(),50.f)){
                float a = atan2(dx,dy);
                deployedtowers[i].setangle(a);
                deployedtowers[i].tryShoot(enemies);        
                deployedtowers[i].updateBullets(dt);
                break;
            }

        }
    }

}

