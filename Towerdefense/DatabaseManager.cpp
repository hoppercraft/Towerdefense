#include <mysql.h>
#include <iostream>
#include <string>

class DatabaseManager {
private:
    MYSQL* connection;

public:
    DatabaseManager() : connection(nullptr) {}

    ~DatabaseManager() {
        if (connection) {
            mysql_close(connection);
        }
    }

    bool connect(const char* host, const char* user, const char* password, const char* database, int port = 3306) {
        connection = mysql_init(nullptr);
        if (!connection) {
            std::cerr << "MySQL initialization failed\n";
            return false;
        }

        if (!mysql_real_connect(connection, host, user, password, database, port, nullptr, 0)) {
            std::cerr << "MySQL connection failed: " << mysql_error(connection) << std::endl;
            mysql_close(connection);
            connection = nullptr;
            return false;
        }

        std::cout << "Successfully connected to MySQL database\n";
        return true;
    }

 

    bool registerUser(const std::string& username, const std::string& password) {
        if (!connection) return false;

        std::string query = "INSERT INTO players (username, password_hash) VALUES ('" +
            username + "', '" + password + "')";

        if (mysql_query(connection, query.c_str())) {
            std::cerr << "Error registering user: " << mysql_error(connection) << std::endl;
            return false;
        }

        std::cout << "User registered successfully\n";
        return true;
    }

    MYSQL* getConnection() {
        return connection;
    }
};