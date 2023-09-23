#include "DBclass.h"
#include <iostream>
#include "sqlite3.h"
#include "symbolSource.h"
#include <vector>
#include "userToken.h"
std::vector<infoStruct> DBclass::getSymbols()
{
	std::vector<infoStruct> symbols;
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(dbSmybol, "select symbol, source, steamhash from symbols", -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		struct infoStruct tempInfoStruct;
		tempInfoStruct.symbol = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		tempInfoStruct.source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempInfoStruct.steamHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		symbols.push_back(tempInfoStruct);
	}
	sqlite3_finalize(stmt);
	return symbols;
}

infoStruct DBclass::validateSymbol(std::string symbol, std::string exchange)
{
	infoStruct retrievedSymbol;
	sqlite3_stmt* stmt;
	std::string sqlStatement = "select * from symbols where symbol = ""\'" + symbol + "\'" + " AND source = ""\'" + exchange + "\'";
	int rc = sqlite3_prepare_v2(dbSmybol, sqlStatement.c_str(), -1, &stmt, 0);
//	this->check_rc(rc);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		struct infoStruct tempInfoStruct;
		retrievedSymbol.symbol = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		retrievedSymbol.source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		retrievedSymbol.steamHash = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
	}
	sqlite3_finalize(stmt);
	retrievedSymbol.symbol.erase(remove(retrievedSymbol.symbol.begin(), retrievedSymbol.symbol.end(), '-'), retrievedSymbol.symbol.end());
	return retrievedSymbol;
}

userToken DBclass::validateToken(std::string token)
{
	userToken userToken;

	sqlite3_stmt* stmt;
	std::string sqlStatement = "select * from tokens where token = ""\'" + token + "\'";
	int rc = sqlite3_prepare_v2(dbSmybol, sqlStatement.c_str(), -1, &stmt, 0);
//	this->check_rc(rc);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		struct infoStruct tempInfoStruct;
		userToken.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		userToken.token = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		userToken.limit = sqlite3_column_double(stmt, 2);
	}
	sqlite3_finalize(stmt);
	if (userToken.name.empty()) {
		userToken.exists = false;
	}
	else {
		userToken.exists = true;
	}

	return userToken;
}

std::vector<infoStruct> DBclass::priceDataRetrieve(std::string symbol, std::string timeframe, std::string source, int limit)
{
	symbol.erase(remove(symbol.begin(), symbol.end(), '-'), symbol.end());
	std::vector<infoStruct> priceData;
	std::string sqlTable = symbol + timeframe + source + " ORDER BY openTime DESC LIMIT " + std::to_string(limit);

	sqlite3_stmt* stmt;
	std::string sqlStatement = "SELECT * from " + sqlTable;
	int rc = sqlite3_prepare_v2(db, sqlStatement.c_str(), -1, &stmt, 0);
//	this->check_rc(rc);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		struct infoStruct tempInfoStruct;
		tempInfoStruct.symbol = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		tempInfoStruct.change = sqlite3_column_double(stmt, 1);
		tempInfoStruct.prcntChange = sqlite3_column_double(stmt, 2);
		tempInfoStruct.high = sqlite3_column_double(stmt, 3);
		tempInfoStruct.low = sqlite3_column_double(stmt, 4);
		tempInfoStruct.last = sqlite3_column_double(stmt, 5);
		tempInfoStruct.open = sqlite3_column_double(stmt, 6);
		tempInfoStruct.volume = sqlite3_column_double(stmt, 7);
		tempInfoStruct.quoteVolume = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
		tempInfoStruct.openTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		tempInfoStruct.source = source;
		priceData.push_back(tempInfoStruct);
	}
	sqlite3_finalize(stmt);

	return priceData;
}
