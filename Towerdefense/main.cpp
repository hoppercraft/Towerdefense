#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameConstants.h"
#include "Towers.h"
#include "Renders.h"
#include "Enemy.h"
#include "Bullet.h"
#include <cstdlib>
#include"gamesession.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({ (Game::MAP_WIDTH + 3) * Game::TILE_SIZE, Game::MAP_HEIGHT * Game::TILE_SIZE }), "Tower Defense Map");
    window.setFramerateLimit(60);

    std::vector<Enemy*> enemies;
    sf::Clock spawnClock;
    float spawnInterval = 1.0f;
    int maxEnemies = 20;
    int spawnedEnemies = 0;

    Shop shop;
    const float speed = 25.0f;

    PlayerInfo playerinfo;
    sf::Clock deltaClock;

    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("Sprites/tile.png")) {
        std::cerr << "Failed to load tile texture.\n";
        return -1;
    }

    sf::IntRect tileRects[4];
    for (int i = 0; i < 4; ++i) {
        tileRects[i] = sf::IntRect({ 20 * i + 1,0 }, { 20,20 });
    }

    sf::Sprite tile(tileTexture);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
            shop.handleEvent(event.value(), window);
        }
        float deltaTime = deltaClock.restart().asSeconds();
        shop.Towertarget(enemies, deltaTime);
        shop.update(window,deltaTime);
        if (spawnedEnemies < maxEnemies && spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            int type = rand() % 2;
            if (type == 0) {
                enemies.push_back(new Enemy());
            }
            else if (type == 1) {
                enemies.push_back(new FastEnemy());
            }
                spawnClock.restart();    
            spawnedEnemies++;
        }


        for (auto& enemy : enemies)
            enemy->update(speed * deltaTime);

        window.clear();

        for (int row = 0; row < Game::MAP_HEIGHT; ++row) {
            for (int col = 0; col < Game::MAP_WIDTH; ++col) {
                int tileType = Game::Map1[row][col];
                tile.setTextureRect(tileRects[tileType]);
                tile.setPosition({ static_cast<float>(col * Game::TILE_SIZE), static_cast<float>(row * Game::TILE_SIZE) });
                window.draw(tile);
            }
        }
        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if (!(*it)->isAlive) {
                delete* it;
                it = enemies.erase(it);
            }
            else {
                (*it)->update(speed * deltaTime);
                if (!(*it)->isMoving) {
                    (*it)->isAlive = false;
                }
                (*it)->draw(window);
                ++it;
            }
        }
        shop.draw(window);
        playerinfo.draw(window);
        window.display();
    }

    return 0;
}
