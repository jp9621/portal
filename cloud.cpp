#include <iostream>
#include <string>
#include <unordered_map>
#include <sqlite3.h>
#include <httplib.h>

std::unordered_map<std::string, std::string> hashData;
sqlite3* db;

int main() {
    // Open SQLite database
    int rc = sqlite3_open("hashes.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Create table if it doesn't exist
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS hashes (id INTEGER PRIMARY KEY AUTOINCREMENT, trade_id TEXT, hash TEXT);";
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // Create and configure server
    httplib::Server svr;
    svr.Get("/verify", [](const httplib::Request& req, httplib::Response& res) {
        auto trade_id = req.get_param_value("trade_id");
        auto hash = req.get_param_value("hash");

        std::string selectQuery = "SELECT COUNT(*) FROM hashes WHERE trade_id='" + trade_id + "' AND hash='" + hash + "';";
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            res.set_content("Error", "text/plain");
            return;
        }

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            if (count > 0) {
                res.set_content("Valid Hash", "text/plain");
            }
            else {
                res.set_content("Invalid Hash", "text/plain");
            }
        }
        else {
            res.set_content("Error", "text/plain");
        }

        sqlite3_finalize(stmt);
        });

    svr.Post("/submit", [](const httplib::Request& req, httplib::Response& res) {
        auto trade_id = req.get_param_value("trade_id");
        auto hash = req.get_param_value("hash");

        std::string insertQuery = "INSERT INTO hashes (trade_id, hash) VALUES ('" + trade_id + "', '" + hash + "');";
        rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            res.set_content("Error", "text/plain");
        }
        else {
            res.set_content("Hash submitted successfully", "text/plain");
        }
        });

    // Start server
    svr.listen("localhost", 8080);

    // Close SQLite database
    sqlite3_close(db);

    return 0;
}
