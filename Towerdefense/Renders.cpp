#include "Renders.h"
#include "Bullet.h"
#include <iostream>
#include <cmath>

Shop::Shop() {
    bar.setPosition(sf::Vector2f(Game::MAP_WIDTH * Game::TILE_SIZE, 0));
    bar.setSize(sf::Vector2f(Game::TILE_SIZE * 3, Game::TILE_SIZE * Game::MAP_HEIGHT));
    bar.setFillColor(sf::Color(0x7C5123FF));
    bar.setOutlineThickness(2.0f);
    bar.setOutlineColor(sf::Color(0xa0acaaFF));

    // Create frames and shop towers for different tower types
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape frame;
        frame.setSize(sf::Vector2f(Game::TILE_SIZE * 1.75f, Game::TILE_SIZE * 1.75f));
        frame.setOrigin(sf::Vector2f(frame.getSize().x / 2.0f, frame.getSize().y / 2.0f));
        frame.setPosition(sf::Vector2f(
            Game::MAP_WIDTH * Game::TILE_SIZE + (bar.getSize().x / 2.0f),
            Game::TILE_SIZE * 1.25f + i * Game::TILE_SIZE * 2.0f
        ));
        frame.setFillColor(sf::Color(0xC4B093FF));
        frames.push_back(frame);

        // Create different tower types for shop display
        sf::Vector2f towerPos = frame.getPosition();
        std::cout << "Creating shop tower " << i << " at position: " << towerPos.x << ", " << towerPos.y << std::endl;

        if (i == 0) {
            // First Basic Tower
            shopTowers.push_back(createTower(TowerType::BASIC, towerPos.x, towerPos.y));
        }
        else if (i == 1) {
            // Second Basic Tower  
            shopTowers.push_back(createTower(TowerType::BASIC, towerPos.x, towerPos.y));
        }
        else if (i == 2) {
            // Tesla Tower
            shopTowers.push_back(createTower(TowerType::TESLA, towerPos.x, towerPos.y));
        }

        // Debug: Check if tower was created successfully
        if (shopTowers.back()) {
            std::cout << "Shop tower " << i << " created successfully, type: " <<
                static_cast<int>(shopTowers.back()->getTowerType()) << std::endl;
        }
    }

    std::cout << "Shop initialized with " << shopTowers.size() << " towers" << std::endl;
}

std::unique_ptr<Tower> Shop::createTower(TowerType type, float x, float y) {
    std::cout << "Creating tower type " << static_cast<int>(type) << " at " << x << ", " << y << std::endl;

    switch (type) {
    case TowerType::BASIC:
        return std::make_unique<BasicTower>(x, y);
    case TowerType::TESLA:
        return std::make_unique<TeslaTower>(x, y);
    default:
        return std::make_unique<BasicTower>(x, y);
    }
}

void Shop::setTowerColor(Tower* tower, ColorState state) {
    if (!tower) {
        std::cout << "Warning: Trying to set color on null tower" << std::endl;
        return;
    }

    std::cout << "Setting tower color to state " << static_cast<int>(state) <<
        " for tower type " << static_cast<int>(tower->getTowerType()) << std::endl;

    // Use virtual functions - no need to cast since they're overridden properly
    switch (state) {
    case ColorState::DEFAULT:
        tower->setfillcolordefault();
        break;
    case ColorState::LIGHT:
        tower->setfillcolorlight();
        break;
    case ColorState::RED:
        tower->setfillcolorred();
        break;
    }
}

void Shop::draw(sf::RenderWindow& window) {
    // Debug: Check window state
    static bool firstDraw = true;
    if (firstDraw) {
        std::cout << "First draw call - window size: " << window.getSize().x << "x" << window.getSize().y << std::endl;
        std::cout << "Drawing " << shopTowers.size() << " shop towers" << std::endl;
        std::cout << "Drawing " << deployedtowers.size() << " deployed towers" << std::endl;
        firstDraw = false;
    }

    // Draw shop background
    window.draw(bar);

    // Draw frames
    for (const auto& frame : frames) {
        window.draw(frame);
    }

    // Draw shop display towers
    for (size_t i = 0; i < shopTowers.size(); ++i) {
        if (shopTowers[i]) {
            shopTowers[i]->draw(window);
        }
        else {
            std::cout << "Warning: Shop tower " << i << " is null" << std::endl;
        }
    }

    // Draw dragged tower
    if (dragging && draggedTower) {
        draggedTower->draw(window);
    }

    // Draw deployed towers
    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        if (deployedtowers[i]) {
            deployedtowers[i]->draw(window);
        }
    }

    // Draw operated tower range (selected tower)
    if (clicked && operatedtower) {
        operatedtower->showrange();
    }
}

void Shop::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f worldPos = mousePos;
    int tileX = static_cast<int>(worldPos.x) / Game::TILE_SIZE;
    int tileY = static_cast<int>(worldPos.y) / Game::TILE_SIZE;

    // Handle mouse button pressed events using SFML 3.0 event system
    if (const auto* mousepressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousepressed->button == sf::Mouse::Button::Left) {
            std::cout << "Mouse clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;

            // Clear previous selection
            if (clicked) {
                setTowerColor(operatedtower, ColorState::DEFAULT);
                clicked = false;
                operatedtower = nullptr;
            }

            // Check if clicking on deployed towers
            for (size_t i = 0; i < deployedtowers.size(); ++i) {
                if (deployedtowers[i]->contain(mousePos)) {
                    std::cout << "Clicked on deployed tower " << i << std::endl;
                    operatedtower = deployedtowers[i].get();
                    clicked = true;
                    return; // Exit early to prevent starting drag
                }
            }

            // Check if clicking on shop towers to start dragging
            for (size_t i = 0; i < shopTowers.size(); ++i) {
                if (shopTowers[i] && shopTowers[i]->contain(mousePos)) {
                    std::cout << "Started dragging shop tower " << i <<
                        " of type " << static_cast<int>(shopTowers[i]->getTowerType()) << std::endl;

                    dragging = true;
                    // Create a new tower of the same type for dragging
                    draggedTower = createTower(shopTowers[i]->getTowerType(), mousePos.x, mousePos.y);

                    if (draggedTower) {
                        setTowerColor(draggedTower.get(), ColorState::LIGHT);
                        std::cout << "Dragged tower created successfully at: " << mousePos.x << ", " << mousePos.y << std::endl;
                    }
                    else {
                        std::cout << "ERROR: Failed to create dragged tower!" << std::endl;
                        dragging = false;
                    }
                    break;
                }
            }
        }
    }
    // Handle mouse button released events
    else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left && dragging && draggedTower) {
            std::cout << "Mouse released - attempting to place tower of type " <<
                static_cast<int>(draggedTower->getTowerType()) << std::endl;

            dragging = false;

            // Check if placement is valid
            if (tileX >= 0 && tileX < Game::MAP_WIDTH && tileY >= 0 && tileY < Game::MAP_HEIGHT) {
                std::cout << "Valid tile position: " << tileX << ", " << tileY << std::endl;

                if (Game::Map1[tileY][tileX] == Game::TileType::Grass) {
                    if (!bounded()) {
                        std::cout << "Placing tower at tile: " << tileX << ", " << tileY << std::endl;

                        // Calculate exact tile center position
                        sf::Vector2f tileCenter(
                            tileX * Game::TILE_SIZE + Game::TILE_SIZE / 2.0f,
                            tileY * Game::TILE_SIZE + Game::TILE_SIZE / 2.0f
                        );

                        // Set final position and default color
                        draggedTower->setposition(tileCenter);
                        setTowerColor(draggedTower.get(), ColorState::DEFAULT);

                        // Move to deployed towers
                        deployedtowers.push_back(std::move(draggedTower));
                        std::cout << "Tower successfully placed and deployed!" << std::endl;
                    }
                    else {
                        std::cout << "Cannot place - too close to another tower" << std::endl;
                    }
                }
                else {
                    std::cout << "Cannot place - not on grass tile (tile type: " <<
                        static_cast<int>(Game::Map1[tileY][tileX]) << ")" << std::endl;
                }
            }
            else {
                std::cout << "Invalid tile position: " << tileX << ", " << tileY << std::endl;
            }

            // Clear the dragged tower regardless of placement success
            draggedTower.reset();
        }
    }

    // Update dragged tower color and position during mouse movement
    if (dragging && draggedTower) {
        // Update position
        draggedTower->setposition(mousePos);

        // Update color based on placement validity
        if (tileX >= 0 && tileX < Game::MAP_WIDTH && tileY >= 0 && tileY < Game::MAP_HEIGHT) {
            if (Game::Map1[tileY][tileX] == Game::TileType::Grass && !bounded()) {
                setTowerColor(draggedTower.get(), ColorState::LIGHT);
            }
            else {
                setTowerColor(draggedTower.get(), ColorState::RED);
            }
        }
        else {
            setTowerColor(draggedTower.get(), ColorState::RED);
        }
    }
}

bool Shop::bounded() {
    if (!draggedTower) return false;

    sf::Vector2f draggedPos = draggedTower->gettowerposition();
    float minDistance = 50.0f; // Minimum distance between towers

    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        sf::Vector2f deployedPos = deployedtowers[i]->gettowerposition();
        float dx = draggedPos.x - deployedPos.x;
        float dy = draggedPos.y - deployedPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= minDistance) {
            return true; // Too close
        }
    }
    return false; // Safe to place
}

void Shop::update(const sf::RenderWindow& window, float dt) {
    // Update deployed towers
    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        if (deployedtowers[i]) {
            deployedtowers[i]->updateBullets(dt);

            // Update Tesla tower lightning effects
            if (deployedtowers[i]->getTowerType() == TowerType::TESLA) {
                TeslaTower* teslaTower = static_cast<TeslaTower*>(deployedtowers[i].get());
                teslaTower->updateLightning();
            }
        }
    }
}

void Shop::Towertarget(std::vector<Enemy*>& enemies, float dt) {
    for (size_t i = 0; i < deployedtowers.size(); ++i) {
        if (!deployedtowers[i]) continue;

        if (deployedtowers[i]->getTowerType() == TowerType::TESLA) {
            // Tesla tower: Fire at ALL enemies in range using its own range
            deployedtowers[i]->tryShoot(enemies);
        }
        else {
            // Basic tower: Find first enemy in range and target it
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (deployedtowers[i]->isInRange(enemies[j]->getposition(), deployedtowers[i]->getrange())) {
                    sf::Vector2f center = deployedtowers[i]->gettowerposition();
                    sf::Vector2f target = enemies[j]->getposition();
                    float dx = center.x - target.x;
                    float dy = target.y - center.y;
                    float angle = atan2(dx, dy);
                    deployedtowers[i]->setangle(angle);
                    deployedtowers[i]->tryShoot(enemies);
                    break; // Basic towers only target first enemy found
                }
            }
        }
    }
}