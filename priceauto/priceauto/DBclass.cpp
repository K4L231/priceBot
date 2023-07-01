#include "DBclass.h"
#include <iostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include "symbolSource.h"

DBclass::DBclass()
{
	sqlite3_open("myDb.db", &db);
	const char* create_table = "CREATE TABLE IF NOT EXISTS min(symbol text, change numeric, high numeric, low numeric, last numeric, open numeric, volume numeric)";
	this->rc = sqlite3_exec(db, create_table, NULL, NULL, &this->err);
	this->check_rc(this->rc);
	this->getSymbols();
}

void DBclass::insert()
{
	std::string symbol = "ETHUSDT";
	double test = 5.666;

	std::string sql("INSERT INTO min (symbol, change, high, low, last, open, volume) VALUES ("
		+ this->quotesql("BTCUSDT") + ","
		+ this->quotesql(std::to_string(test)) + ","
		+ this->quotesql("2") + ","
		+ this->quotesql("2") + ","
		+ this->quotesql("2") + ","
		+ this->quotesql("2") + ","
		+ this->quotesql("1") + ");");

	this->rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &this->err);
	this->check_rc(this->rc);
}

void DBclass::getSymbols()
{
	const char* sql{ "SELECT * FROM symbols" };
	char* err;

	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, "select symbol, source from symbols", -1, &stmt, 0);
	
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		struct infoStruct tempInfoStruct;
		tempInfoStruct.symbol = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		tempInfoStruct.source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		this->symbols.push_back(tempInfoStruct);
	}

	this->check_rc(rc);
}




