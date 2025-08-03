#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameConstants.h"
#include "Towers.h"
#include "Renders.h"
#include "Enemy.h"
#include "Bullet.h"
#include <cstdlib>
#include"gamesession.h"
#include "SoundManager.h"
#include"WaveManager.h"
#include<SFML/Audio.hpp>
#include <mysql.h>
#include "GameSaveManager.h"

// Declare external variables that are defined in Login.cpp
extern MYSQL* globalConnection;
extern std::string loggedInUsername;
extern bool isNewGame;

void runGame() {
    sf::RenderWindow window(sf::VideoMode({ (Game::MAP_WIDTH + 3) * Game::TILE_SIZE, Game::MAP_HEIGHT * Game::TILE_SIZE }), "Tower Defense Map");
    window.setFramerateLimit(60);

    std::vector<Enemy*> enemies;
    WaveManager waveManager;

    sf::Music backgroundMusic;
    bool musicLoaded = false;

    // Load bullet sounds using SoundManager
    if (!SoundManager::getInstance().loadSounds()) {
        std::cerr << "Warning: Could not load bullet sounds. Game will continue without bullet sound effects.\n";
    }

    sf::Clock spawnClock;
    float spawnInterval = 2.f;
    int maxEnemies = 20;
    int spawnedEnemies = 0;
    int currentHighScore = 0;

    Shop shop;
    const float speed = 7.0f;
    PlayerInfo playerinfo;

    // Use the global variables from Login.cpp
    PlayerInfo::setCurrentUser(loggedInUsername, globalConnection);
    GameSaveManager saveManager(loggedInUsername, globalConnection);

    if (isNewGame) {
        // Starting a completely new game
        std::cout << "Starting new game for " << loggedInUsername << std::endl;

        // Delete any existing save file for this user to ensure fresh start
        if (saveManager.hasSaveFile()) {
            std::cout << "Deleting previous save file for new game..." << std::endl;
            saveManager.deleteSaveFile(); // You might need to add this method to GameSaveManager
        }
    }
    else {
        // Continue from saved game
        if (saveManager.hasSaveFile()) {
            std::cout << "Loading previous game for " << loggedInUsername << std::endl;
            saveManager.loadGame(playerinfo, waveManager, shop);
        }
        else {
            std::cout << "Warning: Continue selected but no save file found. Starting new game." << std::endl;
        }
    }

    sf::Clock deltaClock;

    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("Sprites/tile.png")) {
        std::cerr << "Failed to load tile texture.\n";
    }

    if (backgroundMusic.openFromFile("Sounds\\background.wav")) {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(40);
        backgroundMusic.play();
        musicLoaded = true;
        std::cout << "Background music started.\n";
    }
    else {
        std::cerr << "Could not load background music.\n";
    }

    sf::IntRect tileRects[4];
    for (int i = 0; i < 4; ++i) {
        tileRects[i] = sf::IntRect({ 20 * i + 1,0 }, { 20,20 });
    }

    sf::Sprite tile(tileTexture);

    sf::Font font;
    if (!font.openFromFile("font\\Arial.ttf")) {
        std::cerr << "Failed to load font file!\n";
    }
    sf::Text gameovertext(font, "GAME OVER", 170);
    gameovertext.setScale({ 0.105f, 0.105f });
    gameovertext.setStyle(sf::Text::Bold);
    gameovertext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE / 2 - 50, Game::MAP_HEIGHT * Game::TILE_SIZE / 2 - 50 });
    gameovertext.setFillColor(sf::Color(255, 0, 0));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                if (!playerinfo.gameover()) {
                    saveManager.saveGame(playerinfo, waveManager, shop);
                    saveManager.updateHighScore(globalConnection, loggedInUsername, currentHighScore);
                }
                window.close();
            }

            else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                    if (!playerinfo.gameover()) {
                        saveManager.saveGame(playerinfo, waveManager, shop);
                        saveManager.updateHighScore(globalConnection, loggedInUsername, currentHighScore);
                    }
                    window.close();
                }
                else if (keypressed->scancode == sf::Keyboard::Scancode::F5) {
                    // Manual save
                    if (!playerinfo.gameover()) {
                        saveManager.saveGame(playerinfo, waveManager, shop);
                        bool newHighScore = saveManager.updateHighScore(globalConnection, loggedInUsername, currentHighScore);
                        if (newHighScore) {
                            std::cout << "Game saved manually with NEW HIGH SCORE!\n";
                        }
                        else {
                            std::cout << "Game saved manually!\n";
                        }
                    }
                }
                else if (keypressed->scancode == sf::Keyboard::Scancode::Space) {
                    waveManager.startNextWave();
                }
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

            shop.handleEvent(event.value(), window, &playerinfo);
        }

        SoundManager::getInstance().update();

        if (!playerinfo.gameover()) {
            float deltaTime = deltaClock.restart().asSeconds();
            waveManager.update(deltaTime, enemies);
            saveManager.autoSaveIfNeeded(playerinfo, waveManager, shop);
            shop.Towertarget(enemies, deltaTime);
            shop.update(window, deltaTime);

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

            // Clean up dead enemies and handle enemy logic
            for (auto it = enemies.begin(); it != enemies.end(); ) {
                if (!(*it)->isAlive) {
                    playerinfo.coinsearned((*it)->coindropped, (*it)->coindropped);
                    delete* it;
                    it = enemies.erase(it);
                }
                else {
                    (*it)->update(speed * deltaTime);
                    if (!(*it)->isMoving) {
                        playerinfo.enemypassed(1);
                        (*it)->isAlive = false;
                        delete* it;
                        it = enemies.erase(it);
                        continue;
                    }
                    (*it)->draw(window);
                    ++it;
                }
            }
        }

        shop.draw(window);
        playerinfo.draw(window);
        waveManager.draw(window);

        // Handle victory condition
        if (waveManager.allWavesComplete()) {
            static bool victoryShown = false;
            if (!victoryShown) {
                std::cout << "Victory! All waves completed!\n";
                victoryShown = true;
                // Save the victory state
                if (!playerinfo.gameover()) {
                    saveManager.saveGame(playerinfo, waveManager, shop);
                    saveManager.updateHighScore(globalConnection, loggedInUsername, currentHighScore);
                }
            }
        }

        // Handle game over
        if (playerinfo.gameover()) {
            // Save final score when game over occurs (if not already saved)
            static bool gameOverProcessed = false;
            if (!gameOverProcessed) {
                saveManager.saveGame(playerinfo, waveManager, shop);
                // Call the high score update function from GameSaveManager
                bool newHighScore = saveManager.updateHighScore(globalConnection, loggedInUsername, currentHighScore);
                gameOverProcessed = true;

                if (newHighScore) {
                    std::cout << "Game Over - New High Score Achieved!" << std::endl;
                }
            }

            // Check if it's a new high score before showing game over
            if (playerinfo.isNewHighScore()) {
                sf::Text highScoreMessage(font, "NEW HIGH SCORE!", 150);
                highScoreMessage.setScale({ 0.105f, 0.105f });
                highScoreMessage.setStyle(sf::Text::Bold);
                highScoreMessage.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE / 2 - 80, Game::MAP_HEIGHT * Game::TILE_SIZE / 2 - 80 });
                highScoreMessage.setFillColor(sf::Color(255, 215, 0)); // Gold color
                window.draw(highScoreMessage);
            }
            window.draw(gameovertext);
        }

        window.display();
    }

    // Clean up
    for (auto* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    PlayerInfo::closeDatabase();
    if (musicLoaded) {
        backgroundMusic.stop();
    }

    SoundManager::getInstance().cleanup();
}