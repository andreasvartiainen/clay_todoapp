#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <print>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <vector>

using todoFormat = struct {
	uint32_t ID;
	std::string content;
	bool completed;
};


std::vector<todoFormat> todoList{};

class Database{
private:
	sqlite3 *mDB{};
	sqlite3_stmt *mStmt{};
	char *mErrorMsg{};
public:
	bool mRefresh{true};

	Database() {
		std::string fileName = "./sqlite.db";

		// try to open database and create one if it doesn't work
		if (sqlite3_open_v2(fileName.c_str(), &mDB, SQLITE_OPEN_READWRITE, "unix") != SQLITE_OK) {
			if (sqlite3_open_v2(fileName.c_str(), &mDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, "unix") != SQLITE_OK) {
				sqlite3_close(mDB);
				throw std::runtime_error("Error creating database file.");
			}
			createSchema();
		}

		// set foreign_key support on
		sqlite3_exec(mDB, 
				"\
				PRAGMA foreign_keys = ON; \
				"
				, nullptr, nullptr, &mErrorMsg);
		if (mErrorMsg) {
			sqlite3_close(mDB);
			throw std::runtime_error(mErrorMsg);
		}
	}

	static int callback(void *unused, int count, char **data, char **columns) {
		for (int index{0}; index < count; index++) {
			std::println("column: {} data: {}", columns[index], data[index]);
		}
		return 0;
	}

	static int callbackTodo(void *unused, int count, char **data, char **columns) {
		todoFormat currentTodo;
		for (int index{0}; index < count; index++) {
			std::string currentCol{columns[index]};
			std::string currentData{data[index]};
			std::println("column: {} data: {}", currentCol, currentData);
			if (currentCol == "rowid") {
				currentTodo.ID = std::stoi(currentData);
			}
			if (currentCol == "content") {
				currentTodo.content = currentData;
			}
			if (currentCol == "completed") {
				currentTodo.completed = (std::stoi(currentData) == 1);
			}
		}
		todoList.emplace_back(currentTodo);
		return 0;
	}

	void completeTodo(std::string todoID) {
		std::string sqlRequest{std::format("UPDATE todo SET completed = not completed WHERE rowid = {}", todoID)};
		sqlite3_exec(mDB,sqlRequest.c_str(), nullptr, nullptr, &mErrorMsg);
		if (mErrorMsg) {
			sqlite3_close(mDB);
			throw std::runtime_error(mErrorMsg);
		}
		mRefresh = true;
	}

	void removeTodo(std::string todoID) {
		std::string sqlRequest{std::format("DELETE FROM todo WHERE rowid = {}", todoID)};
		sqlite3_exec(mDB,sqlRequest.c_str(), nullptr, nullptr, &mErrorMsg);
		if (mErrorMsg) {
			sqlite3_close(mDB);
			throw std::runtime_error(mErrorMsg);
		}
		mRefresh = true;
	}

	void fetchTodoTable() {
		todoList.clear();
		sqlite3_exec(mDB, "SELECT ROWID, * FROM todo WHERE owner_id IN (SELECT id FROM user WHERE name = 'TestUser')", callbackTodo, nullptr, &mErrorMsg);
		if (mErrorMsg) {
			sqlite3_close(mDB);
			throw std::runtime_error(mErrorMsg);
		}
		mRefresh = false;
	}

	void insertTodo(const std::string todo) {
		// sql request make with format string
		std::string sqlRequest{std::format("INSERT INTO todo(owner_id, content, completed) VALUES((SELECT id FROM user WHERE name = 'TestUser'), '{}', FALSE)", todo)};

		sqlite3_exec(mDB, sqlRequest.c_str(), nullptr, nullptr, &mErrorMsg);
		if (mErrorMsg) {
			sqlite3_close(mDB);
			throw std::runtime_error(mErrorMsg);
		}
		mRefresh = true;
	}

	void fetchUserTable() {
		sqlite3_exec(mDB, "SELECT * FROM user", callback, nullptr, &mErrorMsg);
		if (mErrorMsg) {
			sqlite3_close(mDB);
			throw std::runtime_error(mErrorMsg);
		}
		mRefresh = false;
	}

	void insertUser() {
		sqlite3_exec(mDB, "INSERT INTO user(id, name) VALUES(20, 'TestUser')", nullptr, nullptr, &mErrorMsg);
		if (mErrorMsg) {
			sqlite3_close(mDB);
			throw std::runtime_error(mErrorMsg);
		}
		mRefresh = true;
	}

	// prefer using the sqlite3_exec instead of the separate statements
	void createSchema() {
		sqlite3_exec(mDB, 
				"\
				PRAGMA foreign_keys = ON; \
				CREATE TABLE user(id INT UNIQUE PRIMARY KEY, name TEXT); \
				CREATE TABLE todo(owner_id, content TEXT, completed, FOREIGN KEY(owner_id) REFERENCES user(id)); \
				"
				, nullptr, nullptr, &mErrorMsg);
		if (mErrorMsg) {
			sqlite3_close(mDB);
			throw std::runtime_error(mErrorMsg);
		}
	}

	// creates the schema for the database
	void createSchema_old() {
		if (sqlite3_prepare_v2(mDB, "CREATE TABLE user(age INT, name TEXT, id INT PRIMARY KEY ASC)", -1, &mStmt, nullptr) != SQLITE_OK) {
			sqlite3_close(mDB);
			throw std::runtime_error("Error preparint sqlite statement.");
		}
		// BUG Possible
		if(sqlite3_step(mStmt) != SQLITE_DONE) {
			// std::print("Executing statement.");
			sqlite3_close(mDB);
			throw std::runtime_error("Error executing sqlite statement.");
		}
		if(sqlite3_finalize(mStmt) != SQLITE_OK) {
			sqlite3_close(mDB);
			throw std::runtime_error("Error cleaning up the statement.");
		}
	}


	~Database() {
		sqlite3_close(mDB);
	}
};
