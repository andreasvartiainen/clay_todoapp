#include <format>
#include <functional>
#include <print>
#include <sqlite3.h>
#include <stdexcept>
#include <string>

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

	static int callback(void *unused, int count, char **data, char **columns) {
		for (int index{0}; index < count; index++) {
			std::println("column: {} data: {}", columns[index], data[index]);
		}
		return 0;
	}

	void fetchTodoTable() {
		sqlite3_exec(db, "SELECT * FROM todo WHERE owner_id IN (SELECT id FROM user WHERE name = 'TestUser')", callback, nullptr, &errorMsg);
		if (errorMsg) {
			sqlite3_close(db);
			throw std::runtime_error(errorMsg);
		}
	}

	void insertTodo(const std::string todo) {
		// sql request make with format string
		std::string sqlRequest{std::format("INSERT INTO todo(owner_id, content, completed) VALUES((SELECT id FROM user WHERE name = 'TestUser'), '{}', FALSE)", todo)};

		sqlite3_exec(db, sqlRequest.c_str(), nullptr, nullptr, &errorMsg);
		if (errorMsg) {
			sqlite3_close(db);
			throw std::runtime_error(errorMsg);
		}
	}

	void fetchUserTable() {
		sqlite3_exec(db, "SELECT * FROM user", callback, nullptr, &errorMsg);
		if (errorMsg) {
			sqlite3_close(db);
			throw std::runtime_error(errorMsg);
		}
	}

	void insertUser() {
		sqlite3_exec(db, "INSERT INTO user(id, name) VALUES(20, 'TestUser')", nullptr, nullptr, &errorMsg);
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
				CREATE TABLE user(id INT UNIQUE PRIMARY KEY, name TEXT); \
				CREATE TABLE todo(owner_id, content TEXT, completed, FOREIGN KEY(owner_id) REFERENCES user(id)); \
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
