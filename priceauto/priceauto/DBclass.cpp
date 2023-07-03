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

void DBclass::getSymbols()
{
	const char* create_table = "CREATE TABLE IF NOT EXISTS symbols(symbol text, source text)";
	this->rc = sqlite3_exec(db, create_table, NULL, NULL, &this->err);
	this->check_rc(this->rc);

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

	if (symbols.size() == 0) {
		std::vector <struct infoStruct> tempInfoVector;
		struct infoStruct tempInfoStruct;
		tempInfoStruct.symbol = "BTCUSDT";
		tempInfoStruct.source = "binance";
		tempInfoVector.push_back(tempInfoStruct);

		tempInfoStruct.symbol = "ETHUSDT";
		tempInfoVector.push_back(tempInfoStruct);

		tempInfoStruct.symbol = "DOGEUSDT";
		tempInfoVector.push_back(tempInfoStruct);

		for (int i = 0; i < tempInfoVector.size(); i++) {
			insertToSymbols(tempInfoVector[i].symbol, tempInfoVector[i].source);
		}
	}

	this->check_rc(rc);
}




