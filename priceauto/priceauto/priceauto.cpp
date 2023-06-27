#include <iostream>
#include "sqlite3.h"
#include <string>

std::string quotesql(const std::string& s) {
	return std::string("'") + s + std::string("'");
}

int main() {

	char* err;
	sqlite3* db;
	sqlite3_stmt* stmt;
	sqlite3_open("myDb.db", &db);

	const char* create_table = "CREATE TABLE IF NOT EXISTS min(symbol text, change numeric, high numeric, low numeric, last numeric, open numeric, volume numeric)";
	int rc = sqlite3_exec(db, create_table, NULL, NULL, &err);

	if (rc != SQLITE_OK) {
		std::cout << "err: " << err;
	}


	std::string symbol = "ETHUSDT";

	double test = 5.666;


	std::string sql("INSERT INTO min (symbol, change, high, low, last, open, volume) VALUES ("
		+ quotesql("BTCUSDT") + ","
		+ quotesql(std::to_string(test)) + ","
		+ quotesql("2") + ","
		+ quotesql("2") + ","
		+ quotesql("2") + ","
		+ quotesql("2") + ","
		+ quotesql("1") + ");");
	rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &err);

	if (rc != SQLITE_OK) {
		std::cout << "err: " << err;
	}
}
