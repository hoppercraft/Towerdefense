#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include "GameConstants.h"
#include "Towers.h"
#include "Renders.h"
#include "Enemy.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({ (Game::MAP_WIDTH + 3) * Game::TILE_SIZE, Game::MAP_HEIGHT * Game::TILE_SIZE }), "Tower Defense Map");
    window.setFramerateLimit(60);

    std::vector<Enemy> enemies;
    std::vector<Tower> towers; // Tower vector for placed towers

    sf::Clock spawnClock;
    float spawnInterval = 1.0f;
    int maxEnemies = 20;
    int spawnedEnemies = 0;

    Shop shop;
    const float speed = 25.0f;
    sf::Clock deltaClock;

    // Variables for tower selection and range display
    int selectedTowerIndex = -1;
    bool showSelectedTowerRange = false;

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

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                // Example: Press Space to damage first enemy (for testing)
                else if (keypressed->scancode == sf::Keyboard::Scancode::Space) {
                    if (!enemies.empty()) {
                        enemies[0].takeDamage(25.0f);
                        std::cout << "Enemy health: " << enemies[0].getHealth() << "/" << enemies[0].getMaxHealth() << std::endl;
                    }
                }
                // Add tower placement with T key (for testing)
                else if (keypressed->scancode == sf::Keyboard::Scancode::T) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    towers.emplace_back(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                    std::cout << "Tower placed at: " << mousePos.x << ", " << mousePos.y << std::endl;
                }
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {  // Fixed here
                    sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x),
                        static_cast<float>(mousePressed->position.y));

                    // Check if clicking on a tower
                    selectedTowerIndex = -1;
                    showSelectedTowerRange = false;

                    for (size_t i = 0; i < towers.size(); ++i) {
                        if (towers[i].contain(mousePos)) {
                            selectedTowerIndex = static_cast<int>(i);
                            showSelectedTowerRange = true;
                            std::cout << "Selected tower " << i << std::endl;
                            break;
                        }
                    }
                }
            }

            shop.handleEvent(event.value(), window);
        }

        float deltaTime = deltaClock.restart().asSeconds();
        shop.update(window);

        // Spawn enemies
        if (spawnedEnemies < maxEnemies && spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            enemies.emplace_back();
            spawnClock.restart();
            spawnedEnemies++;
        }

        // Update enemies
        for (auto& enemy : enemies)
            enemy.update(speed * deltaTime);

        // Update towers (automatic targeting and firing)
        for (auto& tower : towers) {
            tower.update(deltaTime, enemies);
            tower.updateBullets(deltaTime, enemies);
        }

        // Remove dead enemies
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& enemy) { return !enemy.isAlive(); }),
            enemies.end()
        );

        // Rendering
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

        // Draw towers with conditional range display
        for (size_t i = 0; i < towers.size(); ++i) {
            bool showRange = (showSelectedTowerRange && selectedTowerIndex == static_cast<int>(i));
            towers[i].draw(window, showRange);
            towers[i].drawBullets(window);
        }

        // Draw enemies with health bars
        for (auto& enemy : enemies)
            enemy.draw(window);

        shop.draw(window);
        
        window.display();
    }

    return 0;
}