#include <SFML/Graphics.hpp>
#include "gamesession.h"
#include"GameConstants.h"
#include <mysql.h>
#include <iostream>

std::string PlayerInfo::currentUsername = "";
MYSQL* PlayerInfo::dbConnection = nullptr;

void PlayerInfo::setCurrentUser(const std::string& username, MYSQL* conn) {
    currentUsername = username;
    dbConnection = conn;
}

bool PlayerInfo::saveScoreToDatabase() {
    if (!dbConnection || currentUsername.empty()) {
        std::cerr << "No database connection or username not set" << std::endl;
        return false;
    }

    // First, get the user_id from the players table
    std::string getUserQuery = "SELECT user_id FROM players WHERE username='" + currentUsername + "'";
    if (mysql_query(dbConnection, getUserQuery.c_str()) != 0) {
        std::cerr << "Error getting user ID: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    MYSQL_RES* res = mysql_store_result(dbConnection);
    if (!res) {
        std::cerr << "Error storing result: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        std::cerr << "User not found in database" << std::endl;
        mysql_free_result(res);
        return false;
    }

    int user_id = std::stoi(row[0]);
    mysql_free_result(res);

    // Check the user's current highest score
    std::string checkScoreQuery = "SELECT MAX(score) FROM scores WHERE user_id=" + std::to_string(user_id);
    if (mysql_query(dbConnection, checkScoreQuery.c_str()) != 0) {
        std::cerr << "Error checking previous scores: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    MYSQL_RES* scoreRes = mysql_store_result(dbConnection);
    if (!scoreRes) {
        std::cerr << "Error storing score result: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    MYSQL_ROW scoreRow = mysql_fetch_row(scoreRes);
    int previousHighScore = 0;

    if (scoreRow && scoreRow[0]) {
        previousHighScore = std::stoi(scoreRow[0]);
    }
    mysql_free_result(scoreRes);

    // Only save if current score is higher than previous best
    if (score > previousHighScore) {
        // Insert the new high score
        std::string insertScoreQuery = "INSERT INTO scores (user_id, score, level_reached, recorded_at) VALUES(" +
            std::to_string(user_id) + ", " +
            std::to_string(score) + ", " +
            "1, " +  // Default level_reached value
            "NOW())";

        if (mysql_query(dbConnection, insertScoreQuery.c_str()) != 0) {
            std::cerr << "Error inserting score: " << mysql_error(dbConnection) << std::endl;
            return false;
        }

        std::cout << "NEW HIGH SCORE! " << score << " (Previous best: " << previousHighScore << ") saved for user " << currentUsername << std::endl;
        return true;
    }
    else {
        std::cout << "Score " << score << " not saved. Not higher than previous best: " << previousHighScore << std::endl;
        return false; // Not an error, just not a high score
    }
}

bool PlayerInfo::isNewHighScore() {
    if (!dbConnection || currentUsername.empty()) {
        return false; // Can't check, assume it's not
    }

    // Get the user_id from the players table
    std::string getUserQuery = "SELECT user_id FROM players WHERE username='" + currentUsername + "'";
    if (mysql_query(dbConnection, getUserQuery.c_str()) != 0) {
        return false;
    }

    MYSQL_RES* res = mysql_store_result(dbConnection);
    if (!res) {
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        mysql_free_result(res);
        return false;
    }

    int user_id = std::stoi(row[0]);
    mysql_free_result(res);

    // Check the user's current highest score
    std::string checkScoreQuery = "SELECT MAX(score) FROM scores WHERE user_id=" + std::to_string(user_id);
    if (mysql_query(dbConnection, checkScoreQuery.c_str()) != 0) {
        return false;
    }

    MYSQL_RES* scoreRes = mysql_store_result(dbConnection);
    if (!scoreRes) {
        return false;
    }

    MYSQL_ROW scoreRow = mysql_fetch_row(scoreRes);
    int previousHighScore = 0;

    if (scoreRow && scoreRow[0]) {
        previousHighScore = std::stoi(scoreRow[0]);
    }
    mysql_free_result(scoreRes);

    return score > previousHighScore;
}



PlayerInfo::PlayerInfo(int coin, int health)
    : coins(coin), health(health), cointext(font, std::to_string(coin), 80),score(0), scoretext(font, "Score:" + std::to_string(score),80),hearttext(font,std::to_string(health), 80), warningtext(font, "", 170) {
    font.openFromFile("font\\ARIAL.ttf");
    hearttext.setScale({ 0.105f, 0.105f });
    hearttext.setFillColor(sf::Color::White);
    hearttext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + 30 ,Game::MAP_HEIGHT * Game::TILE_SIZE - 15 });

    scoretext.setScale({ 0.105f, 0.105f });
    scoretext.setFillColor(sf::Color::White);
    scoretext.setPosition({ Game::MAP_WIDTH/2-5 ,Game::MAP_WIDTH*2});
    scoretext.setString("Score:" + std::to_string(score));

    heartShape.setPointCount(9);
    heartShape.setPoint(0, sf::Vector2f(110.f, -40.f));
    heartShape.setPoint(1, sf::Vector2f(180.f, 40.f)); 
    heartShape.setPoint(2, sf::Vector2f(200.f, 80.f));   
    heartShape.setPoint(3, sf::Vector2f(150.f, 150.f)); 
    heartShape.setPoint(4, sf::Vector2f(100.f, 120.f)); 
    heartShape.setPoint(5, sf::Vector2f(50.f, 150.f));   
    heartShape.setPoint(6, sf::Vector2f(0.f, 80.f));   
    heartShape.setPoint(7, sf::Vector2f(20.f, 40.f));
    heartShape.setPoint(8, sf::Vector2f(90.f, -40.f));
    heartShape.setRotation(sf::degrees(180.f));
    heartShape.setScale({ 0.06f,0.06f });
    heartShape.setFillColor(sf::Color(255, 50, 50));
    heartShape.setOutlineThickness(10.0f);
    heartShape.setOutlineColor(sf::Color::Black);

    heartShape.setPosition({
        hearttext.getPosition().x - 4,
        hearttext.getPosition().y+10
        });
    cointext.setScale({ 0.105f, 0.105f });
    cointext.setFillColor(sf::Color::White);
    cointext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + 30 ,Game::MAP_HEIGHT * Game::TILE_SIZE - 30 });
    coinShape.setRadius(5.f);
    coinShape.setFillColor(sf::Color(255, 215, 0));
    coinShape.setPointCount(10);
    coinShape.setOutlineColor(sf::Color(218, 165, 32));
    coinShape.setOutlineThickness(1.5f);
    coinShape.setPosition({ cointext.getPosition().x - 15,cointext.getPosition().y });

    warningtext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE / 2 - 50, Game::MAP_HEIGHT * Game::TILE_SIZE / 2 -50});
    warningtext.setScale({ 0.105f, 0.105f });
    warningtext.setFillColor(sf::Color(255, 0, 0));
    warningtext.setStyle(sf::Text::Bold);
}



void PlayerInfo::draw(sf::RenderWindow& window) {
    window.draw(cointext);
    window.draw(coinShape);
    window.draw(hearttext);
    window.draw(heartShape);
    window.draw(scoretext);
    if(timer.getElapsedTime().asSeconds() <= 1.f) {
        window.draw(warningtext);
    }
}

void PlayerInfo::turrentplaced(int cost) {
    coins -= cost;
    cointext.setString(std::to_string(coins));
}

void PlayerInfo::coinsearned(int m,int n) {
    coins += m;
    cointext.setString(std::to_string(coins));
    score += n;
    scoretext.setString("Score:" + std::to_string(score));
}

void PlayerInfo::enemypassed(int healthlost) {
    health -= healthlost;
    hearttext.setString(std::to_string(health));
}

bool PlayerInfo::enoughmoney(int cost) {
    if (coins >= cost) {
        return(true);
    }
    else {
        return(false);
    }
    
}

void PlayerInfo::notEnoughMoneywarning() {
    warningtext.setString("Not enough money");
    timer.restart();
}

void PlayerInfo::closeDatabase() {
    std::cout << "Database closed (or simulated close).\n";
}

void PlayerInfo::gameoverr() {
    if (health <= 0) {
        if (!saveScoreToDatabase()) {
            std::cerr << "Failed to save score to database!" << std::endl;
        }
        gameover = true;
    }
}