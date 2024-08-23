#include <string>
#include<sqlite3>
bool validateUser(const std::string& username, const std::string& password) {
    sqlite3* DB;
    sqlite3_open("users.db", &DB);

    std::string sql = "SELECT * FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        int step = sqlite3_step(stmt);
        if (step == SQLITE_ROW) {
            sqlite3_finalize(stmt);
            sqlite3_close(DB);
            return true;  // User found
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return false;  // User not found
}
