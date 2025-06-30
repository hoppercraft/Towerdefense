#include "Enemy.h"

Enemy::Enemy() {
    int startX = 0, startY = 3;
    shape.setRadius(7);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition({ static_cast<float>(startX * Game::TILE_SIZE - 10), static_cast<float>(startY * Game::TILE_SIZE + 10) });
    shape.setOrigin(shape.getGeometricCenter());

    for (int y = 0; y < Game::MAP_HEIGHT; ++y)
        for (int x = 0; x < Game::MAP_WIDTH; ++x)
            visited[y][x] = false;

    findPath(startX, startY);
}

void Enemy::update(float speed) {
    if (currentStep < path.size()) {
        sf::Vector2f target(static_cast<float>(path[currentStep].x * Game::TILE_SIZE + 10),
            static_cast<float>(path[currentStep].y * Game::TILE_SIZE + 10));
        sf::Vector2f current = shape.getPosition();
        sf::Vector2f delta = target - current;

        float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        if (dist < speed) {
            shape.setPosition(target);
            currentStep++;
        }
        else {
            delta /= dist;
            shape.move(delta * speed);
        }
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Enemy::findPath(int x, int y) {
    if (x < 0 || y < 0 || x >= Game::MAP_WIDTH || y >= Game::MAP_HEIGHT)
        return;
    if (Game::Map1[y][x] != Game::TileType::Path || visited[y][x])
        return;

    visited[y][x] = true;
    path.push_back({ x, y });

    for (auto dir : directions)
        findPath(x + dir.x, y + dir.y);

    if (path.size() == 1 || x != path[0].x || y != path[0].y)
        return;

    sf::Vector2i last = path.back();
    path.push_back({ last.x + 1, last.y });
}
sf::FloatRect Enemy::getGlobalBounds() {
    return shape.getGlobalBounds();
}
sf::Vector2f Enemy::getposition() {
    return shape.getPosition();
}
