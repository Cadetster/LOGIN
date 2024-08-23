#include <iostream>
#include <sqlite3.h>
#include <string>

// Function to open the SQLite database
sqlite3* openDatabase(const char* dbName) {
    sqlite3* db;
    int result = sqlite3_open(dbName, &db);
    if (result != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    return db;
}

// Function to close the SQLite database
void closeDatabase(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
    }
}

// Function to authenticate a user by checking their credentials
bool authenticateUser(sqlite3* DB, const std::string& username, const std::string& password) {
    const char* selectSQL = "SELECT password FROM login WHERE username = ?;";
    sqlite3_stmt* stmt;
    bool authenticated = false;

    int exit = sqlite3_prepare_v2(DB, selectSQL, -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* storedPassword = sqlite3_column_text(stmt, 0);
        if (password == reinterpret_cast<const char*>(storedPassword)) {
            authenticated = true;
        }
    } else {
        std::cerr << "User not found." << std::endl;
    }

    sqlite3_finalize(stmt);
    return authenticated;
}

int main() {
    sqlite3* DB = openDatabase("users.db");
    if (!DB) {
        return -1;
    }

    std::string username;
    std::string password;

    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (authenticateUser(DB, username, password)) {
        std::cout << "Login successful!" << std::endl;
    } else {
        std::cout << "Invalid username or password!" << std::endl;
    }

    closeDatabase(DB);
    return 0;
}
