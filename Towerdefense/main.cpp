#include <SFML/Graphics.hpp>
#include <iostream>

enum TileType {
    Grass = 0,
    Water = 1,
    Path = 2,
    Tree = 3
};

const int TILE_SIZE = 20;   
const int MAP_WIDTH = 13;
const int MAP_HEIGHT = 8;

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 3, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 3, 0, 0, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 3, 3, 0, 2, 2, 2, 2, 2, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

class Enemy {
public:
    Enemy() {
        int startX = 0,startY = 3;
        shape.setRadius(7);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition({ static_cast<float>(startX * TILE_SIZE - 10), static_cast<float>(startY * TILE_SIZE + 10) });
        shape.setOrigin(shape.getGeometricCenter());
        for (int y = 0; y < MAP_HEIGHT; ++y)
            for (int x = 0; x < MAP_WIDTH; ++x)
                visited[y][x] = false;

        findPath(startX, startY);
    }

    void update(float speed) {
        if (currentStep < path.size()) {
            sf::Vector2f target(static_cast<float>(path[currentStep].x * TILE_SIZE + 10), static_cast<float>(path[currentStep].y * TILE_SIZE + 10));
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

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

private:
    sf::CircleShape shape;
    std::vector<sf::Vector2i> path;
    bool visited[MAP_HEIGHT][MAP_WIDTH];
    size_t currentStep = 0;

    const std::vector<sf::Vector2i> directions = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}
    };
        
    void findPath(int x, int y) {
        if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT)
            return;
        if (map[y][x] != TileType::Path || visited[y][x])
            return;

        visited[y][x] = true;
        path.push_back({ x, y });

        for (auto dir : directions)
            findPath(x + dir.x, y + dir.y);
        if (path.size() == 1 || x != path[0].x || y != path[0].y)
            return;

        sf::Vector2i last = path.back();
        path.push_back({ last.x+1, last.y });
    }

};
class Shop {
    public:
        Shop() {
            bar.setPosition({ MAP_WIDTH * TILE_SIZE, 0 });
            bar.setSize({ TILE_SIZE * 3, TILE_SIZE * MAP_HEIGHT });
            bar.setFillColor(sf::Color(0x7C5123FF));
            bar.setOutlineThickness({ 2.0f });
            bar.setOutlineColor(sf::Color(0xa0acaaFF));
            for (int i = 0; i < 3; ++i) {
                sf::RectangleShape frame;
                frame.setSize({ TILE_SIZE * 1.75, TILE_SIZE * 1.75 });
                frame.setOrigin(frame.getGeometricCenter());
                frame.setPosition({
                    MAP_WIDTH * TILE_SIZE + bar.getGeometricCenter().x,
                    TILE_SIZE * 1.25f + i * TILE_SIZE * 2.0f
                    });
                frame.setFillColor(sf::Color(0xC4B093FF));
                frames.push_back(frame);
                sf::CircleShape tower;
                tower.setRadius(7);
                tower.setFillColor(sf::Color(0x0000FFFF));
                tower.setOrigin(tower.getGeometricCenter());
                tower.setPosition(frame.getPosition());
                towers.push_back(tower);
            }
        }
        
        void draw(sf::RenderWindow& window) {
            window.draw(bar);
            for(const auto& frame :frames)
            window.draw(frame);
            for (const auto& tower : towers)
                window.draw(tower);
            if (dragging)
                window.draw(draggedTower);
            for (const auto& tower : deployedtowers)
                window.draw(tower);
        }

        void handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (const auto* mousepressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mousepressed->button == sf::Mouse::Button::Left) {
                    for (size_t i = 0; i < towers.size(); ++i) {
                        if (towers[i].getGlobalBounds().contains(mousePos)) {
                            dragging = true;
                            draggedTower = towers[i];
                            draggedTower.setFillColor(sf::Color(0x0000FF55));
                            draggedTower.setPosition(mousePos);
                            break;
                        }
                    }
                }

            }
            else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseReleased->button == sf::Mouse::Button::Left && dragging) {
                    dragging = false;
                    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    int tileX = static_cast<int>(worldPos.x) / TILE_SIZE;
                    int tileY = static_cast<int>(worldPos.y) / TILE_SIZE;
                    
                    if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT) {
                        if (map[tileY][tileX] == TileType::Grass){
                            draggedTower.setFillColor(sf::Color(0x0000FFFF));
                            deployedtowers.push_back(draggedTower);
                        }

                    }
                }
            }
        }
        void update(const sf::RenderWindow& window) {
            if (dragging) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                draggedTower.setPosition(mousePos);
            }
        }
    private:
        std::vector<sf::RectangleShape> frames;
        std::vector<sf::CircleShape> towers;
        std::vector<sf::CircleShape> deployedtowers;
        sf::RectangleShape bar;
        bool dragging = false;
        sf::CircleShape draggedTower;
};
class Tower {
public:
    Tower(sf::Vector2f position) {
        body.setSize({ 10.f, 15.f });
        body.setFillColor(sf::Color(100, 100, 200));
        body.setOrigin({ body.getGeometricCenter().x,head.getRadius() });
        body.setPosition(position);
        head.setRadius(7.f);
        head.setFillColor(sf::Color(0x0000FFFF));
        head.setOrigin(head.getGeometricCenter());
        head.setPosition(position);
        
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(body);
        window.draw(head);
    }

private:
    sf::RectangleShape body;
    sf::CircleShape head;
};
int main() {
    sf::RenderWindow window(sf::VideoMode({ (MAP_WIDTH+3) * TILE_SIZE, MAP_HEIGHT * TILE_SIZE }), "Tower Defense Map");
    window.setFramerateLimit(60);
    Enemy enemy;
    Shop shop;
    Tower tower({ 30.0f, 40.0f });
    const float speed = 25.0f;

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

        shop.update(window);
        enemy.update(speed * deltaTime);
        window.clear();

        for (int row = 0; row < MAP_HEIGHT; ++row) {
            for (int col = 0; col < MAP_WIDTH; ++col) {
                int tileType = map[row][col];
                tile.setTextureRect(tileRects[tileType]);
                tile.setPosition({ static_cast<float>(col * TILE_SIZE), static_cast<float>(row * TILE_SIZE) });
                window.draw(tile);
                window.draw(tile);
            }
        }
       
        enemy.draw(window);
        shop.draw(window);
        tower.draw(window);
        window.display();
    }

    return 0;
}
