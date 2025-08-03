#pragma once
#include <SFML/Graphics.hpp>
#include <mysql.h>

class PlayerInfo {
public:
    PlayerInfo(int coin = 500, int health = 5);
    void draw(sf::RenderWindow& window);
    void turrentplaced(int cost);
    void enemypassed(int healthlost);
    bool enoughmoney(int cost);
    void notEnoughMoneywarning();
    void coinsearned(int m,int n=0);
    bool gameover();
    static void setCurrentUser(const std::string& username, MYSQL* conn);
    bool saveScoreToDatabase();
    int getCurrentScore() const { return score; }
    int getCurrentCoins() const { return coins; }
    int getCurrentHealth() const { return health; }
    static void closeDatabase();
    bool isNewHighScore();

    void setCoins(int newCoins) {
        coins = newCoins;
        cointext.setString(std::to_string(coins));
    }
    void setHealth(int newHealth) {
        health = newHealth;
        hearttext.setString(std::to_string(health));
    }

    void setScore(int newScore) {
        score = newScore;
        scoretext.setString("Score:" + std::to_string(score));
    }

private:
    sf::Font font;
    sf::Text cointext;
    sf::Text hearttext;
    sf::Text warningtext,scoretext;
    sf::CircleShape coinShape;
    sf::ConvexShape heartShape;
    int coins;
    int health;
    int score;
    int i;
    sf::Clock timer;
    sf::Time offset;
    sf::Time elapsed;
    static std::string currentUsername; 
    static MYSQL* dbConnection;
};