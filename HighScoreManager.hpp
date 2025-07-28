#ifndef HIGHSCOREMANAGER_HPP
#define HIGHSCOREMANAGER_HPP

#include <string>
#include <vector>

struct ScoreEntry {
    std::string username;
    int score;
};

class HighScoreManager {
public:
    HighScoreManager(const std::string& filename);

    int getHighScore(const std::string& username);
    void updateScore(const std::string& username, int newScore);
    std::vector<ScoreEntry> getTopScores(int count = 5);
    
private:
    std::string filename;
    std::vector<ScoreEntry> scores;

    void loadScores();
    void saveScores();
};

#endif // HIGHSCOREMANAGER_HPP
