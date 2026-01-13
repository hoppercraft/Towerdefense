#pragma once
#include <mysql.h>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include "WaveManager.h"
#include "Renders.h"
#include "gamesession.h"

struct TowerSaveData {
    std::string towerType;
    float x, y;
    bool upgraded;
    float angle;
};

class GameSaveManager {
public:
    GameSaveManager(const std::string& username, MYSQL* connection);

    // Main save/load functions
    bool saveGame(const PlayerInfo& playerInfo, const WaveManager& waveManager, const Shop& shop);
    bool loadGame(PlayerInfo& playerInfo, WaveManager& waveManager, Shop& shop);
    bool hasSaveFile() const;
    void deleteSaveFile();

    // Auto-save functionality
    void autoSaveIfNeeded(const PlayerInfo& playerInfo, const WaveManager& waveManager, const Shop& shop);
    bool updateHighScore(MYSQL* conn, const std::string& username, int& currentHighScore);

private:
    std::string playerUsername;
    MYSQL* dbConnection;
    int userId;
    int lastSavedWave = 0;

    // Helper functions
    bool getUserId();
    bool savePlayerAndWaveData(int coins, int health, int score, int wave, bool waveActive,
         float cooldown, bool inCooldown);
    bool saveTowerData(const std::vector<TowerSaveData>& towers);
    bool loadPlayerAndWaveData(int& coins, int& health, int& score, int& wave, bool& waveActive,
         float& cooldown, bool& inCooldown);
    bool loadTowerData(std::vector<TowerSaveData>& towers);
    std::vector<TowerSaveData> extractTowerData(const Shop& shop);
    void restoreTowers(Shop& shop, const std::vector<TowerSaveData>& towerData);
    std::string escapeString(const std::string& str);
};


