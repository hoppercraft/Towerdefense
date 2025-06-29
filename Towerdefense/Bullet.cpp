#include "Bullet.h"
#include <iostream>

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f targetPos) {
    // Make bullet highly visible - large yellow ball with red outline
    shape.setRadius(8.f); // Large radius for high visibility
    shape.setFillColor(sf::Color::Yellow);
    shape.setOutlineThickness(2.f); // Thick outline
    shape.setOutlineColor(sf::Color::Red); // High contrast red outline
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
    shape.setPosition(startPos);

    position = startPos;
    targetPosition = targetPos;

    // Calculate direction and velocity
    sf::Vector2f direction = targetPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0.0f) {
        velocity = direction / length * 120.f; // Slower speed for better visibility
    }
    else {
        velocity = sf::Vector2f(0.f, 0.f);
    }

    // Debug output - track bullet creation
    std::cout << "=== BULLET CREATED ===" << std::endl;
    std::cout << "Start Position: (" << startPos.x << ", " << startPos.y << ")" << std::endl;
    std::cout << "Target Position: (" << targetPos.x << ", " << targetPos.y << ")" << std::endl;
    std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")" << std::endl;
    std::cout << "Distance to target: " << length << std::endl;
    std::cout << "======================" << std::endl;
}

void Bullet::update(float deltaTime) {
    // Store previous position for debugging
    sf::Vector2f previousPos = position;

    // Update position
    position += velocity * deltaTime;
    shape.setPosition(position);

    // Debug output every 20 frames to track movement
    static int frameCount = 0;
    frameCount++;
    if (frameCount % 20 == 0) {
        std::cout << "Bullet moving: (" << previousPos.x << ", " << previousPos.y
            << ") -> (" << position.x << ", " << position.y << ")" << std::endl;

        // Calculate distance to target
        float distToTarget = std::sqrt(
            (targetPosition.x - position.x) * (targetPosition.x - position.x) +
            (targetPosition.y - position.y) * (targetPosition.y - position.y)
        );
        std::cout << "Distance to target: " << distToTarget << std::endl;
    }
}

void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(shape);

    // Debug confirmation that draw is being called
    static int drawCallCount = 0;
    drawCallCount++;
    if (drawCallCount % 60 == 0) { // Every 60 draw calls (about once per second at 60fps)
        std::cout << "Bullet draw() called " << drawCallCount << " times - Position: ("
            << position.x << ", " << position.y << ")" << std::endl;
    }
}

bool Bullet::reachedTarget() const {
    float distance = std::sqrt(
        (targetPosition.x - position.x) * (targetPosition.x - position.x) +
        (targetPosition.y - position.y) * (targetPosition.y - position.y)
    );

    bool reached = distance < 20.f; // Large tolerance to prevent bullets from disappearing too quickly

    if (reached) {
        std::cout << "*** BULLET REACHED TARGET ***" << std::endl;
        std::cout << "Final distance to target: " << distance << std::endl;
        std::cout << "Bullet position: (" << position.x << ", " << position.y << ")" << std::endl;
        std::cout << "Target position: (" << targetPosition.x << ", " << targetPosition.y << ")" << std::endl;
        std::cout << "****************************" << std::endl;
    }

    return reached;
}

sf::Vector2f Bullet::getPosition() const {
    return position;
}