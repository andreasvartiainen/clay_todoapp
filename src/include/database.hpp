#include <sqlite3.h>
#include <stdexcept>

class Database{
private:
	sqlite3 *db{};
	sqlite3_stmt *stmt{};
	char *errorMsg{};
public:
	Database() {
		std::string fileName = "./sqlite.db";

		// try to open database and create one if it doesn't work
		if (sqlite3_open_v2(fileName.c_str(), &db, SQLITE_OPEN_READWRITE, "unix") != SQLITE_OK) {
			if (sqlite3_open_v2(fileName.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, "unix") != SQLITE_OK) {
				sqlite3_close(db);
				throw std::runtime_error("Error creating database file.");
			}
			createSchema();
		}

		// set foreign_key support on
		sqlite3_exec(db, 
				"\
				PRAGMA foreign_keys = ON; \
				"
				, nullptr, nullptr, &errorMsg);
		if (errorMsg) {
			sqlite3_close(db);
			throw std::runtime_error(errorMsg);
		}

	}

	// prefer using the sqlite3_exec instead of the separate statements
	void createSchema() {
		sqlite3_exec(db, 
				"\
				PRAGMA foreign_keys = ON; \
				CREATE TABLE user(age INT, name TEXT); \
				CREATE TABLE todo(owner_id UNIQUE, content TEXT, completed, FOREIGN KEY(owner_id) REFERENCES user(ROWID)); \
				"
				, nullptr, nullptr, &errorMsg);
		if (errorMsg) {
			sqlite3_close(db);
			throw std::runtime_error(errorMsg);
		}
	}

	// creates the schema for the database
	void createSchema_old() {
		if (sqlite3_prepare_v2(db, "CREATE TABLE user(age INT, name TEXT, id INT PRIMARY KEY ASC)", -1, &stmt, nullptr) != SQLITE_OK) {
			sqlite3_close(db);
			throw std::runtime_error("Error preparint sqlite statement.");
		}
		// BUG Possible
		if(sqlite3_step(stmt) != SQLITE_DONE) {
			// std::print("Executing statement.");
			sqlite3_close(db);
			throw std::runtime_error("Error executing sqlite statement.");
		}
		if(sqlite3_finalize(stmt) != SQLITE_OK) {
			sqlite3_close(db);
			throw std::runtime_error("Error cleaning up the statement.");
		}
	}


	~Database() {
		sqlite3_close(db);
	}
};
