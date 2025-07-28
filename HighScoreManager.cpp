#include "HighScoreManager.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>  // For debug prints

HighScoreManager::HighScoreManager(const std::string& filename)
    : filename(filename) {
    std::cout << "[HighScoreManager] Constructor called with file: " << filename << std::endl;
    loadScores();
}

void HighScoreManager::loadScores() {
    std::cout << "[HighScoreManager] loadScores() skipped for testing.\n";
    // Disable file reading to avoid crashing due to file issues
    // scores.clear();
    // std::ifstream file(filename);
    // if (!file.is_open()) return;

    // std::string name;
    // int score;
    // while (file >> name >> score) {
    //     scores.push_back({name, score});
    // }

    // file.close();
}


void HighScoreManager::saveScores() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "[HighScoreManager] Could not open file to save scores: " << filename << std::endl;
        return;
    }

    std::cout << "[HighScoreManager] Saving " << scores.size() << " scores to file." << std::endl;
    for (const auto& entry : scores) {
        std::cout << "  Saving User: " << entry.username << ", Score: " << entry.score << std::endl;
        file << entry.username << " " << entry.score << "\n";
    }

    file.close();
}

int HighScoreManager::getHighScore(const std::string& username) {
    std::cout << "[HighScoreManager] Getting high score for user: " << username << std::endl;
    for (const auto& entry : scores) {
        if (entry.username == username) {
            std::cout << "[HighScoreManager] Found score: " << entry.score << std::endl;
            return entry.score;
        }
    }
    std::cout << "[HighScoreManager] No score found for user, returning 0" << std::endl;
    return 0;
}

void HighScoreManager::updateScore(const std::string& username, int newScore) {
    std::cout << "[HighScoreManager] Updating score for user: " << username << " with newScore: " << newScore << std::endl;
    bool found = false;
    for (auto& entry : scores) {
        if (entry.username == username) {
            if (newScore > entry.score) {
                std::cout << "[HighScoreManager] New score is higher, updating from " << entry.score << " to " << newScore << std::endl;
                entry.score = newScore;
            } else {
                std::cout << "[HighScoreManager] Existing score is higher or equal, no update." << std::endl;
            }
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "[HighScoreManager] User not found in scores, adding new entry." << std::endl;
        scores.push_back({username, newScore});
    }

    std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return a.score > b.score;
    });

    saveScores();
}

std::vector<ScoreEntry> HighScoreManager::getTopScores(int count) {
    std::vector<ScoreEntry> topScores = scores;
    if (topScores.size() > static_cast<size_t>(count)) {
        topScores.resize(count);
    }
    return topScores;
}
