#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <iostream>
#include "GameConstants.h"
#include "Towers.h"
#include "Renders.h"
#include "Enemy.h"
#include "Bullet.h"
#include "WaveManager.h"  // Add this include
#include <cstdlib>
#include "gamesession.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({ (Game::MAP_WIDTH + 3) * Game::TILE_SIZE, Game::MAP_HEIGHT * Game::TILE_SIZE }), "Tower Defense Map - Multi Tower Edition");
    window.setFramerateLimit(60);

    std::vector<Enemy*> enemies;
    WaveManager waveManager;  // Create wave manager

    Shop shop;
    const float speed = 25.0f;
    PlayerInfo playerinfo;
    sf::Clock deltaClock;

    sf::Music backgroundMusic;
    bool musicLoaded = false;

    // Load tile texture
    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("Sprites/tile.png")) {
        std::cerr << "Failed to load tile texture.\n";
        return -1;
    }
    if (backgroundMusic.openFromFile("D:\\new1\\Sounds\\s.wav.wav")) {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
        musicLoaded = true;
        std::cout << "Background music started.\n";
    }
    else {
        std::cerr << "Could not load background music.\n";
    }

    sf::IntRect tileRects[4];
    for (int i = 0; i < 4; ++i) {
        tileRects[i] = sf::IntRect({ 20 * i + 1, 0 }, { 20, 20 });
    }
    sf::Sprite tile(tileTexture);

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                // Optional: Allow player to start next wave early with spacebar
                else if (keypressed->scancode == sf::Keyboard::Scancode::Space) {
                    waveManager.startNextWave();
                }
                // Optional: Reset game with R key
                else if (keypressed->scancode == sf::Keyboard::Scancode::R) {
                    // Clear all enemies
                    for (auto* enemy : enemies) {
                        delete enemy;
                    }
                    enemies.clear();
                    waveManager.reset();
                }
                // Add this in your existing keyboard event handling
                else if (keypressed->scancode == sf::Keyboard::Scancode::M) {
                    if (musicLoaded) {
                        if (backgroundMusic.getStatus() == sf::SoundSource::Status::Playing) {
                            backgroundMusic.pause();
                        }
                        else {
                            backgroundMusic.play();
                        }
                    }
                }
            }


            // Handle shop events (tower selection, dragging, deployment)
            shop.handleEvent(event.value(), window);
        }

        // Update game state
        float deltaTime = deltaClock.restart().asSeconds();

        // Update wave manager (handles enemy spawning)
        waveManager.update(deltaTime, enemies);

        // Update shop (tower targeting, bullet updates)
        shop.Towertarget(enemies, deltaTime);
        shop.update(window, deltaTime);

        // Update enemies
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

        // Draw shop UI and towers
        shop.draw(window);

        // Draw player info
        playerinfo.draw(window);

        // Draw wave manager UI
        waveManager.draw(window);

        // Check for game over conditions
        if (waveManager.allWavesComplete()) {
            // You could add victory screen logic here
            // For now, just display victory message in console
            static bool victoryShown = false;
            if (!victoryShown) {
                std::cout << "Victory! All waves completed!\n";
                victoryShown = true;
            }
        }

        window.display();
    }
    // Stop music before closing
    if (musicLoaded) {
        backgroundMusic.stop();
    }
    // Clean up enemies
    for (auto* enemy : enemies) {
        delete enemy;
    }

    return 0;
}