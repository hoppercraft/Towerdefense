#include "Enemy.h"

Enemy::Enemy() {
    int startX = 0, startY = 3;
    position = sf::Vector2f(static_cast<float>(startX * Game::TILE_SIZE + Game::TILE_SIZE / 2),
        static_cast<float>(startY * Game::TILE_SIZE + Game::TILE_SIZE / 2));


    sf::Color darkNinja(25, 25, 35);
    sf::Color shadowBlack(12, 12, 18);
    sf::Color maskGray(35, 35, 45);
    sf::Color eyeGlow(0, 255, 255, 200);
    sf::Color steelBlade(180, 185, 190);
    sf::Color goldAccent(200, 165, 32);
    sf::Color leatherBrown(80, 50, 35);

    body.setRadius(5);
    body.setFillColor(darkNinja);
    body.setOrigin({ 5.f, 5.f });


    hood.setRadius(6.5f);
    hood.setFillColor(maskGray);
    hood.setOrigin({ 6.5f, 6.5f });


    faceMask.setRadius(4.5f);
    faceMask.setFillColor(shadowBlack);
    faceMask.setOrigin({ 4.5f, 4.5f });


    leftArm.setSize({ 2.f, 8.f });
    leftArm.setFillColor(darkNinja);
    leftArm.setOrigin({ 1.f, 4.f });

    rightArm.setSize({ 2.f, 8.f });
    rightArm.setFillColor(darkNinja);
    rightArm.setOrigin({ 1.f, 4.f });


    leftHand.setRadius(1.5f);
    leftHand.setFillColor(maskGray);
    leftHand.setOrigin({ 1.5f, 1.5f });

    rightHand.setRadius(1.5f);
    rightHand.setFillColor(maskGray);
    rightHand.setOrigin({ 1.5f, 1.5f });


    leftLeg.setSize({ 2.f, 7.f });
    leftLeg.setFillColor(darkNinja);
    leftLeg.setOrigin({ 1.f, 3.5f });

    rightLeg.setSize({ 2.f, 7.f });
    rightLeg.setFillColor(darkNinja);
    rightLeg.setOrigin({ 1.f, 3.5f });


    leftFoot.setSize({ 3.f, 1.5f });
    leftFoot.setFillColor(shadowBlack);
    leftFoot.setOrigin({ 1.5f, 0.75f });

    rightFoot.setSize({ 3.f, 1.5f });
    rightFoot.setFillColor(shadowBlack);
    rightFoot.setOrigin({ 1.5f, 0.75f });


    leftEye.setRadius(1.2f);
    leftEye.setFillColor(eyeGlow);
    leftEye.setOrigin({ 1.2f, 1.2f });

    rightEye.setRadius(1.2f);
    rightEye.setFillColor(eyeGlow);
    rightEye.setOrigin({ 1.2f, 1.2f });


    leftEyeGlow.setRadius(2.f);
    leftEyeGlow.setFillColor(sf::Color(0, 255, 255, 50));
    leftEyeGlow.setOrigin({ 2.f, 2.f });

    rightEyeGlow.setRadius(2.f);
    rightEyeGlow.setFillColor(sf::Color(0, 255, 255, 50));
    rightEyeGlow.setOrigin({ 2.f, 2.f });


    swordBlade.setSize({ 1.5f, 10.f });
    swordBlade.setFillColor(steelBlade);
    swordBlade.setOrigin({ 0.75f, 5.f });


    swordGuard.setSize({ 4.f, 1.f });
    swordGuard.setFillColor(goldAccent);
    swordGuard.setOrigin({ 2.f, 0.5f });


    swordHandle.setSize({ 1.2f, 3.f });
    swordHandle.setFillColor(leatherBrown);
    swordHandle.setOrigin({ 0.6f, 1.5f });


    swordPommel.setRadius(0.8f);
    swordPommel.setFillColor(goldAccent);
    swordPommel.setOrigin({ 0.8f, 0.8f });


    belt.setSize({ 12.f, 1.5f });
    belt.setFillColor(leatherBrown);
    belt.setOrigin({ 6.f, 0.75f });


    movementTimer = 1.f;
    isMoving = true;


    for (int y = 0; y < Game::MAP_HEIGHT; ++y)
        for (int x = 0; x < Game::MAP_WIDTH; ++x)
            visited[y][x] = false;

    // Find path and update initial position
    findPath(startX, startY);
    updateNinjaPosition(position);
}

void Enemy::update(float deltaTime, float speed) {
    movementTimer += deltaTime;

    if (currentStep < path.size()) {
        sf::Vector2f target(static_cast<float>(path[currentStep].x * Game::TILE_SIZE + Game::TILE_SIZE / 2),
            static_cast<float>(path[currentStep].y * Game::TILE_SIZE + Game::TILE_SIZE / 2));
        sf::Vector2f current = position;
        sf::Vector2f delta = target - current;
        float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (dist < speed * deltaTime) {
            position = target;
            currentStep++;
            if (currentStep >= path.size()) {
                isMoving = false;
            }
        }
        else {
            delta /= dist;
            position += delta * speed * deltaTime;
            isMoving = true;
        }


        updateNinjaPosition(position);
    }
}

void Enemy::updateNinjaPosition(sf::Vector2f newPos) {
    position = newPos;

    float bobOffset = 1.f;
    if (isMoving) {
        bobOffset = std::sin(movementTimer * 0.1f) * 0.5f;
    }


    body.setPosition({ newPos.x, newPos.y + bobOffset });


    hood.setPosition({ newPos.x, newPos.y - 8 + bobOffset });

    faceMask.setPosition({ newPos.x, newPos.y - 8 + bobOffset });


    float armSway = isMoving ? std::sin(movementTimer) * 1.f : 0.f;
    leftArm.setPosition({ newPos.x - 6, newPos.y - 1 + bobOffset });
    rightArm.setPosition({ newPos.x + 6, newPos.y - 1 + bobOffset + armSway });

    leftHand.setPosition({ newPos.x - 6, newPos.y + 3 + bobOffset });
    rightHand.setPosition({ newPos.x + 6, newPos.y + 3 + bobOffset + armSway });


    float legOffset = isMoving ? std::sin(movementTimer) * 0.7f : 0.f;
    leftLeg.setPosition({ newPos.x - 2, newPos.y + 7 + bobOffset - legOffset });
    rightLeg.setPosition({ newPos.x + 2, newPos.y + 7 + bobOffset + legOffset });


    leftFoot.setPosition({ newPos.x - 2, newPos.y + 12 + bobOffset - legOffset });
    rightFoot.setPosition({ newPos.x + 2, newPos.y + 12 + bobOffset + legOffset });


    leftEye.setPosition({ newPos.x - 2, newPos.y - 9 + bobOffset });
    rightEye.setPosition({ newPos.x + 2, newPos.y - 9 + bobOffset });


    leftEyeGlow.setPosition({ newPos.x - 2, newPos.y - 9 + bobOffset });
    rightEyeGlow.setPosition({ newPos.x + 2, newPos.y - 9 + bobOffset });


    float swordSway = isMoving ? std::sin(movementTimer): 0.f;
    swordBlade.setPosition({ newPos.x + 7, newPos.y - 3 + bobOffset + armSway });
    swordGuard.setPosition({ newPos.x + 7, newPos.y + 2 + bobOffset + armSway });
    swordHandle.setPosition({ newPos.x + 7, newPos.y + 4 + bobOffset + armSway });
    swordPommel.setPosition({ newPos.x + 7, newPos.y + 5 + bobOffset + armSway });


    belt.setPosition({ newPos.x, newPos.y + 2 + bobOffset });


    if (isMoving) {
        float swordAngle = std::sin(movementTimer);
        swordGuard.setRotation(sf::degrees(swordAngle));
        swordHandle.setRotation(sf::degrees(swordAngle));
    }
    else {
        swordBlade.setRotation(sf::degrees(0));
        swordGuard.setRotation(sf::degrees(0));
        swordHandle.setRotation(sf::degrees(0));
    }
}

void Enemy::draw(sf::RenderWindow& window) {

    window.draw(leftEyeGlow);
    window.draw(rightEyeGlow);


    window.draw(swordBlade);
    window.draw(swordGuard);
    window.draw(swordHandle);
    window.draw(swordPommel);


    window.draw(leftLeg);
    window.draw(rightLeg);
    window.draw(leftFoot);
    window.draw(rightFoot);


    window.draw(body);
    window.draw(belt);


    window.draw(leftArm);
    window.draw(rightArm);
    window.draw(leftHand);
    window.draw(rightHand);

    window.draw(hood);
    window.draw(faceMask);

    // Eyes on top for glow effect
    window.draw(leftEye);
    window.draw(rightEye);
}

void Enemy::findPath(int x, int y) {
    if (x < 0 || y < 0 || x >= Game::MAP_WIDTH || y >= Game::MAP_HEIGHT)
        return;
    if (Game::Map1[y][x] != Game::TileType::Path || visited[y][x])
        return;

    visited[y][x] = true;
    path.push_back({ x, y });

    // Try all four directions
    for (auto dir : directions)
        findPath(x + dir.x, y + dir.y);


    if (path.size() == 1 || x != path[0].x || y != path[0].y)
        return;

    sf::Vector2i last = path.back();
    path.push_back({ last.x + 1, last.y });
}

sf::FloatRect Enemy::getGlobalBounds() {

    sf::FloatRect bounds = body.getGlobalBounds();
    bounds.size.x += 8;
    bounds.size.y += 20;
    bounds.position.x -= 4;
    bounds.position.y -= 12;
    return bounds;
}

sf::Vector2f Enemy::getposition() {
    return position;
}

FastEnemy::FastEnemy() {
    // Blue color
    faceMask.setFillColor(sf::Color(80, 80, 200));
    body.setFillColor(sf::Color(60, 60, 255));
    hood.setFillColor(sf::Color(100, 120, 255));
}
void FastEnemy::update(float deltaTime) {
    Enemy::update(deltaTime, 5.5f);
}

