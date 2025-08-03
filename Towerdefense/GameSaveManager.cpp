#include "GameSaveManager.h"
#include "Towers.h"

GameSaveManager::GameSaveManager(const std::string& username, MYSQL* connection)
    : playerUsername(username), dbConnection(connection), userId(-1) {
    getUserId();
}

bool GameSaveManager::getUserId() {
    if (!dbConnection) return false;

    std::string query = "SELECT user_id FROM players WHERE username='" + escapeString(playerUsername) + "'";

    if (mysql_query(dbConnection, query.c_str()) != 0) {
        std::cerr << "Error getting user ID: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    MYSQL_RES* result = mysql_store_result(dbConnection);
    if (!result) return false;

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        userId = std::atoi(row[0]);
        mysql_free_result(result);
        return true;
    }

    mysql_free_result(result);
    return false;
}

bool GameSaveManager::saveGame(const PlayerInfo& playerInfo, const WaveManager& waveManager, const Shop& shop) {
    if (userId == -1 || !dbConnection) {
        std::cerr << "Invalid user ID or database connection" << std::endl;
        return false;
    }

    try {
        // Get data using your existing methods and the new getters
        int coins = playerInfo.getCurrentCoins();
        int health = playerInfo.getCurrentHealth();
        int current_score = playerInfo.getCurrentScore();
        int currentWave = waveManager.getCurrentWave();
        bool waveActive = waveManager.isWaveActive();
        float cooldownTime = waveManager.getWaveCooldownTime();
        bool inCooldown = waveManager.isInCooldown();

        // Save to database
        if (!savePlayerAndWaveData(coins, health, current_score, currentWave, waveActive,
            cooldownTime, inCooldown)) {
            return false;
        }

        // Save tower data
        std::vector<TowerSaveData> towerData = extractTowerData(shop);
        if (!saveTowerData(towerData)) {
            return false;
        }

        lastSavedWave = currentWave;
        std::cout << "Game saved successfully! Wave: " << currentWave
            << ", Coins: " << coins << ", Health: " << health
            << ", Towers: " << towerData.size() << std::endl;
        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

bool GameSaveManager::loadGame(PlayerInfo& playerInfo, WaveManager& waveManager, Shop& shop) {
    if (userId == -1 || !dbConnection) {
        std::cerr << "Invalid user ID or database connection" << std::endl;
        return false;
    }

    try {
        int coins, health, current_score, wave;
        bool waveActive, inCooldown;
        float cooldownTime;

        // Load player and wave data
        if (!loadPlayerAndWaveData(coins, health, current_score, wave, waveActive,cooldownTime, inCooldown)) {
            return false;
        }

        // Load tower data
        std::vector<TowerSaveData> towerData;
        if (!loadTowerData(towerData)) {
            return false;
        }

        // Restore game state using your existing methods and the new setters
        playerInfo.setCoins(coins);
        playerInfo.setHealth(health);
        playerInfo.setScore(current_score);

        waveManager.loadSavedWaveState(wave, waveActive, cooldownTime, inCooldown);

        restoreTowers(shop, towerData);

        lastSavedWave = wave;
        std::cout << "Game loaded successfully! Wave: " << wave
            << ", Coins: " << coins << ", Health: " << health
            << ", Towers: " << towerData.size() << std::endl;
        return true;

    }
    catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

bool GameSaveManager::hasSaveFile() const {
    if (userId == -1 || !dbConnection) return false;

    std::string query = "SELECT COUNT(*) FROM game_saves WHERE user_id=" + std::to_string(userId);

    if (mysql_query(dbConnection, query.c_str()) != 0) {
        return false;
    }

    MYSQL_RES* result = mysql_store_result(dbConnection);
    if (!result) return false;

    MYSQL_ROW row = mysql_fetch_row(result);
    bool hasSave = false;

    if (row && std::atoi(row[0]) > 0) {
        hasSave = true;
    }

    mysql_free_result(result);
    return hasSave;
}

void GameSaveManager::deleteSaveFile() {
    if (userId == -1 || !dbConnection) return;

    // Delete towers first (foreign key constraint)
    std::string deleteTowersQuery = "DELETE FROM saved_towers WHERE user_id=" + std::to_string(userId);
    mysql_query(dbConnection, deleteTowersQuery.c_str());

    // Delete game save
    std::string deleteGameQuery = "DELETE FROM game_saves WHERE user_id=" + std::to_string(userId);
    mysql_query(dbConnection, deleteGameQuery.c_str());

    std::cout << "Save data deleted." << std::endl;
}

void GameSaveManager::autoSaveIfNeeded(const PlayerInfo& playerInfo, const WaveManager& waveManager, const Shop& shop) {
    int currentWave = waveManager.getCurrentWave();

    // Auto-save at the end of each wave
    if (currentWave != lastSavedWave && !waveManager.isWaveActive()) {
        saveGame(playerInfo, waveManager, shop);
    }
}

bool GameSaveManager::updateHighScore(MYSQL* conn, const std::string& username, int& currentHighScore)
{
    // First, get the user_id
    std::string userQuery = "SELECT user_id FROM players WHERE username='" + username + "'";
    if (mysql_query(conn, userQuery.c_str()) != 0) {
        std::cout << "Error getting user ID: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_RES* userRes = mysql_store_result(conn);
    if (!userRes) {
        std::cout << "Error storing user result: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_ROW userRow = mysql_fetch_row(userRes);
    if (!userRow) {
        std::cout << "User not found!" << std::endl;
        mysql_free_result(userRes);
        return false;
    }

    int user_id = std::stoi(userRow[0]);
    mysql_free_result(userRes);

    // Get current score from game_saves table
    std::string currentScoreQuery = "SELECT current_score FROM game_saves WHERE user_id=" + std::to_string(user_id);    if (mysql_query(conn, currentScoreQuery.c_str()) != 0) {
        std::cout << "Error getting current score: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_RES* currentRes = mysql_store_result(conn);
    if (!currentRes) {
        std::cout << "Error storing current score result: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_ROW currentRow = mysql_fetch_row(currentRes);
    if (!currentRow || !currentRow[0]) {
        std::cout << "No current score found in game_saves!" << std::endl;
        mysql_free_result(currentRes);
        return false;
    }

    int currentScore = std::stoi(currentRow[0]);
    mysql_free_result(currentRes);

    // Get highest score from scores table
    std::string highScoreQuery = "SELECT MAX(score) FROM scores WHERE user_id=" + std::to_string(user_id);
    if (mysql_query(conn, highScoreQuery.c_str()) != 0) {
        std::cout << "Error getting high score: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_RES* highRes = mysql_store_result(conn);
    if (!highRes) {
        std::cout << "Error storing high score result: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_ROW highRow = mysql_fetch_row(highRes);
    int highestScore = (highRow && highRow[0]) ? std::stoi(highRow[0]) : 0;
    mysql_free_result(highRes);

    // Check if current score is higher than the highest score
    if (currentScore > highestScore) {
        // Insert new high score into scores table
        std::string insertScoreQuery = "INSERT INTO scores (user_id, score) VALUES(" +
            std::to_string(user_id) + ", " +
            std::to_string(currentScore) + ", NOW())";

        if (mysql_query(conn, insertScoreQuery.c_str()) != 0) {
            std::cout << "Error inserting new high score: " << mysql_error(conn) << std::endl;
            return false;
        }

        // Update the current high score variable
        currentHighScore = currentScore;
        std::cout << "New high score achieved: " << currentScore << std::endl;
        return true; // Score was updated
    }
    return false;
}

bool GameSaveManager::savePlayerAndWaveData(int coins, int health, int current_score, int wave, bool waveActive,
   float cooldown, bool inCooldown) {
    std::string query = "INSERT INTO game_saves (user_id, coins, health, current_score, current_wave, wave_active, "
        " cooldown_time, in_cooldown) VALUES (" +
        std::to_string(userId) + ", " +
        std::to_string(coins) + ", " +
        std::to_string(health) + ", " +
        std::to_string(current_score) + ", " +
        std::to_string(wave) + ", " +
        (waveActive ? "TRUE" : "FALSE") + ", " +
        std::to_string(cooldown) + ", " +
        (inCooldown ? "TRUE" : "FALSE") + ") " +
        "ON DUPLICATE KEY UPDATE " +
        "coins=" + std::to_string(coins) + ", " +
        "health=" + std::to_string(health) + ", " +
        "current_score=" + std::to_string(current_score) + ", " +
        "current_wave=" + std::to_string(wave) + ", " +
        "wave_active=" + (waveActive ? "TRUE" : "FALSE") + ", " +
        "cooldown_time=" + std::to_string(cooldown) + ", " +
        "in_cooldown=" + (inCooldown ? "TRUE" : "FALSE") + ", " +
        "saved_at=CURRENT_TIMESTAMP";

    if (mysql_query(dbConnection, query.c_str()) != 0) {
        std::cerr << "Error saving player data: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    return true;
}
bool GameSaveManager::loadPlayerAndWaveData(int& coins, int& health, int& current_score, int& wave, bool& waveActive,
   float& cooldown, bool& inCooldown) {
    std::string query = "SELECT coins, health, current_score, current_wave, wave_active, "
        "cooldown_time, in_cooldown FROM game_saves WHERE user_id=" + std::to_string(userId);

    if (mysql_query(dbConnection, query.c_str()) != 0) {
        std::cerr << "Error loading player data: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    MYSQL_RES* result = mysql_store_result(dbConnection);
    if (!result) return false;

    MYSQL_ROW row = mysql_fetch_row(result);
    if (!row) {
        mysql_free_result(result);
        return false;
    }

    coins = std::atoi(row[0]);
    health = std::atoi(row[1]);
    current_score = std::atoi(row[2]);
    wave = std::atoi(row[3]);
    waveActive = (std::string(row[4]) == "1");
    cooldown = std::atof(row[5]);
    inCooldown = (std::string(row[6]) == "1");

    mysql_free_result(result);
    return true;
}


bool GameSaveManager::saveTowerData(const std::vector<TowerSaveData>& towers) {
    // First, delete existing towers for this user
    std::string deleteQuery = "DELETE FROM saved_towers WHERE user_id=" + std::to_string(userId);
    if (mysql_query(dbConnection, deleteQuery.c_str()) != 0) {
        std::cerr << "Error deleting existing towers: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    // Insert new tower data
    for (const auto& tower : towers) {
        std::string query = "INSERT INTO saved_towers (user_id, tower_type, position_x, position_y, upgraded, angle) VALUES (" +
            std::to_string(userId) + ", '" +
            escapeString(tower.towerType) + "', " +
            std::to_string(tower.x) + ", " +
            std::to_string(tower.y) + ", " +
            (tower.upgraded ? "TRUE" : "FALSE") + ", " +
            std::to_string(tower.angle) + ")";

        if (mysql_query(dbConnection, query.c_str()) != 0) {
            std::cerr << "Error saving tower data: " << mysql_error(dbConnection) << std::endl;
            return false;
        }
    }

    return true;
}

bool GameSaveManager::loadTowerData(std::vector<TowerSaveData>& towers) {
    towers.clear();

    std::string query = "SELECT tower_type, position_x, position_y, upgraded, angle FROM saved_towers WHERE user_id=" + std::to_string(userId);

    if (mysql_query(dbConnection, query.c_str()) != 0) {
        std::cerr << "Error loading tower data: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    MYSQL_RES* result = mysql_store_result(dbConnection);
    if (!result) {
        std::cerr << "Error storing result: " << mysql_error(dbConnection) << std::endl;
        return false;
    }

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        TowerSaveData tower;
        tower.towerType = std::string(row[0]);
        tower.x = std::atof(row[1]);
        tower.y = std::atof(row[2]);
        tower.upgraded = (std::string(row[3]) == "1");
        tower.angle = std::atof(row[4]);
        towers.push_back(tower);
    }

    mysql_free_result(result);
    return true;
}

std::vector<TowerSaveData> GameSaveManager::extractTowerData(const Shop& shop) {
    std::vector<TowerSaveData> towerData;

    // Access deployed towers from shop
    const auto& deployedTowers = shop.getDeployedTowers();

    for (const auto& tower : deployedTowers) {
        TowerSaveData data;

        // Determine tower type using dynamic_cast
        if (dynamic_cast<const CannonTower*>(tower.get())) {
            data.towerType = "CannonTower";
        }
        else if (dynamic_cast<const Boat*>(tower.get())) {
            data.towerType = "Boat";
        }
        else if (dynamic_cast<const TeslaTower*>(tower.get())) {
            data.towerType = "TeslaTower";
        }
        else {
            data.towerType = "Tower"; // Default/base tower
        }

        // Get position and state
        sf::Vector2f pos = tower->gettowerposition();
        data.x = pos.x;
        data.y = pos.y;
        data.upgraded = tower->upgraded;
        data.angle = tower->gettowerangle().asRadians();

        towerData.push_back(data);
    }

    return towerData;
}

void GameSaveManager::restoreTowers(Shop& shop, const std::vector<TowerSaveData>& towerData) {
    // Clear existing deployed towers using the correct method name
    shop.clearAllTowers();

    for (const auto& data : towerData) {
        std::unique_ptr<Tower> tower;

        // Create tower based on type
        if (data.towerType == "CannonTower") {
            tower = std::make_unique<CannonTower>(data.x, data.y);
        }
        else if (data.towerType == "Boat") {
            tower = std::make_unique<Boat>(data.x, data.y);
        }
        else if (data.towerType == "TeslaTower") {
            tower = std::make_unique<TeslaTower>(data.x, data.y);
        }
        else {
            tower = std::make_unique<Tower>(data.x, data.y);
        }

        // Restore tower state
        if (data.upgraded) {
            tower->upgrade();
        }
        tower->setangle(data.angle);

        // Add to deployed towers using the correct method name
        shop.addSavedTower(std::move(tower));
    }

    std::cout << "Restored " << towerData.size() << " towers." << std::endl;
}
// Add these functions to your code



std::string GameSaveManager::escapeString(const std::string& str) {
    std::string escaped;
    escaped.reserve(str.length() * 2);

    for (char c : str) {
        if (c == '\'' || c == '\"' || c == '\\') {
            escaped += '\\';
        }
        escaped += c;
    }

    return escaped;
}