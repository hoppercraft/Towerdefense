#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameConstants.h"
#include "Towers.h"
#include "Renders.h"
#include "Enemy.h"
#include "Bullet.h"
#include "WaveManager.h"
#include <cstdlib>
#include "gamesession.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({ (Game::MAP_WIDTH + 3) * Game::TILE_SIZE, Game::MAP_HEIGHT * Game::TILE_SIZE }), "Tower Defense Map - Multi Tower Edition");
    window.setFramerateLimit(60);

    std::vector<Enemy*> enemies;
    WaveManager waveManager;
    Shop shop;
    const float speed = 25.0f;
    PlayerInfo playerinfo(500, 5);  // Initialize with 500 coins and 5 health
    sf::Clock deltaClock;
    bool gameOverDisplayed = false;

    // Load tile texture
    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("Sprites/tile.png")) {
        std::cerr << "Failed to load tile texture.\n";
        return -1;
    }

    sf::IntRect tileRects[4];
    for (int i = 0; i < 4; ++i) {
        tileRects[i] = sf::IntRect({ 20 * i + 1, 0 }, { 20, 20 });
    }
    sf::Sprite tile(tileTexture);

    // Main game loop
    while (window.isOpen()) {
        // Check for game over condition first
        if (playerinfo.gameover() && !gameOverDisplayed) {
            std::cout << "Game Over! You ran out of health!\n";
            gameOverDisplayed = true;
            // You could add game over screen logic here
        }

        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                // Allow player to start next wave early with spacebar
                else if (keypressed->scancode == sf::Keyboard::Scancode::Space) {
                    waveManager.startNextWave();
                }
                // Reset game with R key
                else if (keypressed->scancode == sf::Keyboard::Scancode::R) {
                    // Clear all enemies
                    for (auto* enemy : enemies) {
                        delete enemy;
                    }
                    enemies.clear();
                    waveManager.reset();
                    // Reset player info
                    playerinfo = PlayerInfo(500, 5);
                    gameOverDisplayed = false;
                }
            }
            // Handle shop events (tower selection, dragging, deployment)
            shop.handleEvent(event.value(), window, playerinfo);  // Pass playerinfo to shop
        }

        // Skip game updates if game is over
        if (playerinfo.gameover()) {
            // Still render the current state but don't update
            window.clear();

            // Draw map tiles
            for (int row = 0; row < Game::MAP_HEIGHT; ++row) {
                for (int col = 0; col < Game::MAP_WIDTH; ++col) {
                    int tileType = Game::Map1[row][col];
                    tile.setTextureRect(tileRects[tileType]);
                    tile.setPosition({ static_cast<float>(col * Game::TILE_SIZE), static_cast<float>(row * Game::TILE_SIZE) });
                    window.draw(tile);
                }
            }

            // Draw remaining enemies
            for (auto& enemy : enemies) {
                enemy->draw(window);
            }

            // Draw shop UI and towers
            shop.draw(window);

            // Draw player info (will show 0 health)
            playerinfo.draw(window);

            // Draw wave manager UI
            waveManager.draw(window);

            window.display();
            continue;
        }

        // Update game state
        float deltaTime = deltaClock.restart().asSeconds();

        // Update wave manager (handles enemy spawning)
        waveManager.update(deltaTime, enemies);

        // Update shop (tower targeting, bullet updates)
        shop.Towertarget(enemies, deltaTime);
        shop.update(window, deltaTime);

        // Update enemies and check for enemies that reached the end
        for (auto& enemy : enemies) {
            enemy->update(speed * deltaTime);
        }

        // Render everything
        window.clear();

        // Draw map tiles
        for (int row = 0; row < Game::MAP_HEIGHT; ++row) {
            for (int col = 0; col < Game::MAP_WIDTH; ++col) {
                int tileType = Game::Map1[row][col];
                tile.setTextureRect(tileRects[tileType]);
                tile.setPosition({ static_cast<float>(col * Game::TILE_SIZE), static_cast<float>(row * Game::TILE_SIZE) });
                window.draw(tile);
            }
        }

        // Update and draw enemies
        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if (!(*it)->isAlive) {
                // Enemy was killed by tower - give coins to player
                playerinfo.coinsearned((*it)->getReward()); // Assuming Enemy has getReward() method
                delete* it;
                it = enemies.erase(it);
            }
            else {
                (*it)->update(speed * deltaTime);
                if (!(*it)->isMoving) {
                    // Enemy reached the end - player loses health
                    playerinfo.enemypassed((*it)->getDamage()); // Assuming Enemy has getDamage() method
                    (*it)->isAlive = false;
                }
                (*it)->draw(window);
                ++it;
            }
        }

        // Draw shop UI and towers
        shop.draw(window);

        // Draw player info
        playerinfo.draw(window);

        // Draw wave manager UI
        waveManager.draw(window);

        // Check for game victory conditions
        if (waveManager.allWavesComplete() && enemies.empty()) {
            static bool victoryShown = false;
            if (!victoryShown) {
                std::cout << "Victory! All waves completed!\n";
                victoryShown = true;
            }
        }

        window.display();
    }

    // Clean up enemies
    for (auto* enemy : enemies) {
        delete enemy;
    }

    return 0;
}