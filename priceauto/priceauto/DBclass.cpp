#include "DBclass.h"
#include <iostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include "symbolSource.h"

DBclass::DBclass()
{
	int rc = sqlite3_open("../../../myDb.db", &db);
	this->check_rc(rc);

	rc = sqlite3_open("../../../DBsymbol.db", &dbSmybol);
	this->check_rc(rc);
	this->getSymbols();
}

void DBclass::getSymbols()
{
	const char* create_table = "CREATE TABLE IF NOT EXISTS symbols(symbol text, source text, steamhash text)";
	this->rc = sqlite3_exec(dbSmybol, create_table, NULL, NULL, &this->err);
	this->check_rc(this->rc);

	const char* sql{ "SELECT * FROM symbols" };

	this->retrieveSymbols();

//	if (symbols.size() == 0) {
//		std::vector <struct infoStruct> tempInfoVector;
//		struct infoStruct tempInfoStruct;
//		tempInfoStruct.symbol = "BTCUSDT";
//		tempInfoStruct.source = "binance";
//		tempInfoVector.push_back(tempInfoStruct);
//
//		tempInfoStruct.symbol = "ETHUSDT";
//		tempInfoVector.push_back(tempInfoStruct);
//
//		tempInfoStruct.symbol = "DOGEUSDT";
//		tempInfoVector.push_back(tempInfoStruct);
//
//		for (int i = 0; i < tempInfoVector.size(); i++) {
//			insertToSymbols(tempInfoVector[i].symbol, tempInfoVector[i].source);
//		}
//		this->retrieveSymbols();
//	}

	this->check_rc(rc);
}
