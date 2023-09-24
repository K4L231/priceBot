#include "DBclass.h"
#include <iostream>
#include "sqlite3.h"
#include "symbolSource.h"
#include <vector>
#include "userToken.h"
#pragma warning(disable : 4996)
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
	int symbolAmount = intervalList[timeframe];
	if (symbolAmount > 60) {
		symbolAmount = 60;
	}
	symbol.erase(remove(symbol.begin(), symbol.end(), '-'), symbol.end());
	std::vector<infoStruct> priceData;
	std::string sqlTable = symbol + "1min" + source + " ORDER BY openTime DESC LIMIT " + std::to_string((limit * symbolAmount) + symbolAmount);
	sqlite3_stmt* stmt;
	std::string sqlStatement = "SELECT * from " + sqlTable;
	int rc = sqlite3_prepare_v2(db, sqlStatement.c_str(), -1, &stmt, 0);
	this->check_rc(rc);
	while ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
//		int rc = sqlite3_step(stmt);
		if (rc == 101) break;
		struct infoStruct tempInfoStruct;
		if (rc == 5) {
			while (rc == 5) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				std::cout << "SLEEEPING ------------" << std::endl;
				rc = sqlite3_step(stmt);
			}
		}
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
//		std::cout << "time: " << tempInfoStruct.openTime << std::endl;
	}
	sqlite3_finalize(stmt);
	std::vector<infoStruct> newPriceData;
	struct infoStruct tempInfoStruct;
	tempInfoStruct.symbol = priceData[0].symbol;
	tempInfoStruct.source = priceData[0].source;
	tempInfoStruct.steamHash = priceData[0].steamHash;
	tempInfoStruct.low = 99999999999;
	tempInfoStruct.high = -1;
	tempInfoStruct.quoteVolume = "0";
	tempInfoStruct.volume = 0;
	int last = 0;
	int symbolAmountHour = intervalListHour[timeframe];

	int currentHour = -1;
	for (int i = 0; i < priceData.size(); i++) {

		if (priceData[i].low < tempInfoStruct.low) {
			tempInfoStruct.low = priceData[i].low;
		}
		if (priceData[i].high > tempInfoStruct.high) {
			tempInfoStruct.high = priceData[i].high;
		}
		tempInfoStruct.volume = tempInfoStruct.volume + priceData[i].volume;
		tempInfoStruct.quoteVolume = std::to_string(std::stoll(tempInfoStruct.quoteVolume) + std::stoll(priceData[i].quoteVolume));

		if (this->convertOpenTimeToMin(priceData[i].openTime) % symbolAmount == 0) {
			tempInfoStruct.openTime = priceData[i].openTime;
			tempInfoStruct.open = priceData[i].open;
			tempInfoStruct.last = priceData[last].last;
			tempInfoStruct.change = tempInfoStruct.last - tempInfoStruct.open;
			tempInfoStruct.prcntChange = (tempInfoStruct.change * 100) / tempInfoStruct.last;
			newPriceData.push_back(tempInfoStruct);
			// RESET tempInfoStruct;
			last = i + 1;
			tempInfoStruct.low = 999999999999;
			tempInfoStruct.high = -1;
			tempInfoStruct.volume = 0;
			tempInfoStruct.quoteVolume = "0";
		}
		
		if (intervalList[timeframe] < 60 && newPriceData.size() >= limit) {
			break;
		}
	}
	
	if (intervalList[timeframe] >= 60) {
		std::vector<infoStruct> newPriceDataHour;
		struct infoStruct tempInfoStruct;
		tempInfoStruct.symbol = newPriceData[0].symbol;
		tempInfoStruct.source = newPriceData[0].source;
		tempInfoStruct.steamHash = newPriceData[0].steamHash;
		tempInfoStruct.low = 99999999999;
		tempInfoStruct.high = -1;
		tempInfoStruct.quoteVolume = "0";
		tempInfoStruct.volume = 0;
		int last = 0;
		int symbolAmountHour = intervalListHour[timeframe];
		for (int i = 0; i < newPriceData.size(); i++) {
			if (newPriceData[i].low < tempInfoStruct.low) {
				tempInfoStruct.low = newPriceData[i].low;
			}
			if (newPriceData[i].high > tempInfoStruct.high) {
				tempInfoStruct.high = newPriceData[i].high;
			}
			tempInfoStruct.volume = tempInfoStruct.volume + newPriceData[i].volume;
			tempInfoStruct.quoteVolume = std::to_string(std::stoll(tempInfoStruct.quoteVolume) + std::stoll(newPriceData[i].quoteVolume));

			if (this->convertOpenTimeToHour(newPriceData[i].openTime) % symbolAmountHour == 0) {
				tempInfoStruct.openTime = newPriceData[i].openTime;
				tempInfoStruct.open = newPriceData[i].open;
				tempInfoStruct.last = newPriceData[last].last;
				tempInfoStruct.change = tempInfoStruct.last - tempInfoStruct.open;
				tempInfoStruct.prcntChange = (tempInfoStruct.change * 100) / tempInfoStruct.last;
				newPriceDataHour.push_back(tempInfoStruct);
				// RESET tempInfoStruct;
				last = i + 1;
				tempInfoStruct.low = 999999999999;
				tempInfoStruct.high = -1;
				tempInfoStruct.volume = 0;
				tempInfoStruct.quoteVolume = "0";
			}

			if (newPriceDataHour.size() >= limit) {
				break;
			}
		}
		return newPriceDataHour;
	}

	return newPriceData;
}
