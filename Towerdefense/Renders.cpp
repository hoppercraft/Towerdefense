#include "Renders.h"
#include"Bullet.h"
#include <iostream>
#include <cmath>
#include"gamesession.h"
#include"Towers.h"
Shop::Shop() :shopTowername(font,"", 80) {
    font.openFromFile("ARIAL.ttf");
    bar.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE, 0 });
    bar.setSize({ Game::TILE_SIZE * 3, Game::TILE_SIZE * Game::MAP_HEIGHT });
    bar.setFillColor(sf::Color(0x7C5123FF));
    bar.setOutlineThickness({ 2.0f });
    bar.setOutlineColor(sf::Color(0xa0acaaFF)); 

    shopTowername.setScale({ 0.105f, 0.105f });
    shopTowername.setFillColor(sf::Color::White);
    shopTowername.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + bar.getGeometricCenter().x-6,Game::TILE_SIZE * 1.5f-10 });
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
        if (i == 0) {
            towers.push_back(std::make_unique<CannonTower>(frame.getPosition().x, frame.getPosition().y));
        }
        else{
             towers.push_back(std::make_unique<Boat>(frame.getPosition().x, frame.getPosition().y));
        }
    }

}

void Shop::draw(sf::RenderWindow& window) {
    window.draw(bar);
    for (const auto& frame : frames)
        window.draw(frame);
    for (const auto& tower : towers)
        tower->draw(window);
    if (dragging && draggedTower)
        draggedTower->draw(window);
    for (const auto& tower : deployedtowers)
        tower->draw(window);
    if (clicked) {
        operatedtower->draw(window);
        window.draw(shopbar);
        shopTower->draw(window);
        window.draw(shopTowername);
    }
}


void Shop::handleEvent(const sf::Event& event, const sf::RenderWindow& window,PlayerInfo* pinfo) {
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
                if (deployedtowers[i]->contain(mousePos)) {
                    operatedtower = deployedtowers[i].get();
                    shopTower = deployedtowers[i]->clone();
                    operatedtower->showrange();
                    Towerupgradeshop(pinfo);
                    clicked = true;
                    break;
                }
            }
            for (size_t i = 0; i < towers.size(); ++i) {
                if (towers[i]->contain(mousePos)) {
                    dragging = true;
                    draggedTower = towers[i]->clone();
                    draggedTower->setfillcolorlight();
                    draggedTower->setposition(mousePos);
                    break;
                }
            }
        }
    }
    else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left && dragging) {
            dragging = false;
            if (tileX >= 0 && tileX < Game::MAP_WIDTH && tileY >= 0 && tileY < Game::MAP_HEIGHT) {
                
                if (Game::Map1[tileY][tileX] == draggedTower->towertilereq()) {
                    if (Shop::bounded() == false) {
                        if (pinfo->enoughmoney(100)) {
                            draggedTower->setfillcolordefault();
                            deployedtowers.push_back(draggedTower->clone());
                            pinfo->turrentplaced(100);
                        }
                        else {
                            pinfo->notEnoughMoneywarning();
                        }
                    }
                }
            }
        }
    }
    if (dragging && draggedTower) {
        if (tileX >= 0 && tileX < Game::MAP_WIDTH && tileY >= 0 && tileY < Game::MAP_HEIGHT) {
            
            if(draggedTower)
            if (Game::Map1[tileY][tileX] == draggedTower->towertilereq() && Shop::bounded() == false) {
                draggedTower->setfillcolorlight();
            }
            else {
                draggedTower->setfillcolorred();
            }
        }
    }
}

bool Shop::bounded() {
    if (!draggedTower) return false;
    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        sf::Vector2f center = deployedtowers[i]->gettowerposition();
        sf::Vector2f target = draggedTower->gettowerposition();
        float dx = center.x - target.x;
        float dy = target.y - center.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance<= deployedtowers[i]->getradius()*3) {
            return true;
        }
    }
    return false;
}

void Shop::update(const sf::RenderWindow& window,float dt) {
    if (dragging && draggedTower) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        draggedTower->setposition(mousePos);
    }
    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        deployedtowers[i]->updateBullets(dt);
    }
}

void Shop::Towertarget(std::vector<Enemy*>& enemies,float dt){
    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        for (size_t j = 0; j < enemies.size(); ++j) {
            sf::Vector2f center = deployedtowers[i]->gettowerposition();
            sf::Vector2f target = enemies[j]->getposition();
            float dx = center.x - target.x;
            float dy = target.y - center.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (deployedtowers[i]->isInRange(enemies[j]->getposition(),deployedtowers[i]->getrange())) {
                float a = atan2(dx,dy);
                deployedtowers[i]->setangle(a);
                deployedtowers[i]->tryShoot(enemies);
                break;
            }

        }

    }

}

void Shop::Towerupgradeshop(PlayerInfo* info) {
    shopbar.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE, 0 });
    shopbar.setSize({ Game::TILE_SIZE * 3, Game::TILE_SIZE * (Game::MAP_HEIGHT-1) });
    shopbar.setFillColor(sf::Color(0x7C5123FF));
    shopTower->setposition({ Game::MAP_WIDTH * Game::TILE_SIZE + bar.getGeometricCenter().x,Game::TILE_SIZE * 2.0f });
    shopTower->setangle(0.f);
    shopTower->clearBullets();
    shopTowername.setString(shopTower->gettowername());    
    shopTowername.setOrigin({ shopTowername.getLocalBounds().getCenter().x / 2.0f,
       shopTowername.getLocalBounds().getCenter().y / 2.0f
        });
}