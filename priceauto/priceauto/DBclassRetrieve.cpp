#include "DBclass.h"


void DBclass::deleteLastRow(std::string symbol, std::string timeframe, std::string source)
{
//	std::cout << timeframe << std::endl;
	infoStruct tempInfoStruct;
	std::string sqlQuery = "select symbol, change, prcntChange, high, low, last, open, volume, quoteVolume, openTime from " + symbol + timeframe + source + " ORDER BY openTime DESC LIMIT 1";
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, 0);
	this->check_rc(rc);
	if (rc == 0) {
		while (sqlite3_step(stmt) != SQLITE_DONE) {
			tempInfoStruct.source = source;
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

		}
	}

	sqlite3_finalize(stmt);

//	std::cout << " symbol: " << tempInfoStruct.symbol << std::endl;
//	std::cout << " source: " << tempInfoStruct.source << std::endl;
//	std::cout << " change: " << tempInfoStruct.change << std::endl;
//	std::cout << " high: " <<   tempInfoStruct.high << std::endl;
//	std::cout << " low: " <<    tempInfoStruct.low << std::endl;
//	std::cout << " time: " <<   tempInfoStruct.openTime << std::endl;

	infoStruct item = tempInfoStruct;
	std::string deleteQuery = "DELETE FROM " + symbol + timeframe + source + " where " +
		"symbol = " + "\'" + item.symbol + "\'" +
		" AND change = " + std::to_string(item.change) +
		" AND prcntChange = " + std::to_string(item.prcntChange) +
		" AND high = " + std::to_string(item.high) +
		" AND low = " + std::to_string(item.low) +
		" AND last = " + std::to_string(item.last) +
		" AND open = " + std::to_string(item.open) +
		" AND volume = " + std::to_string(item.volume) +
		" AND quoteVolume = " + "\'" + item.quoteVolume + "\'" +
		" AND openTime = " + "\'" + item.openTime + "\'"
		+ ";";
	this->rc = sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, &this->err);
	this->check_rc(this->rc);
	return;
}


void DBclass::retrievePriceInfo(std::string symbol, std::string timeframe, std::vector<infoStruct>& priceInfoVector, std::string source, int numberOfRows)
{
	std::string sqlQuery = "select symbol, change, prcntChange, high, low, last, open, volume, quoteVolume, openTime from " + symbol + timeframe + source;
	sqlite3_stmt* stmt;
	int rc = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, 0);
	if (rc == SQLITE_OK) {
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
			priceInfoVector.push_back(tempInfoStruct);
//			priceInfoVector.insert(priceInfoVector.begin(), tempInfoStruct);
		}
	}
	this->cleanUpData(priceInfoVector, symbol, timeframe);
	sqlite3_finalize(stmt);
	return;
}

void DBclass::cleanUpData(std::vector<infoStruct>& priceInfoVector, std::string symbol, std::string timeframe) {
	int maxNumberOfRows = 60000;

	if (priceInfoVector.size() > maxNumberOfRows) {

		infoStruct item = priceInfoVector[priceInfoVector.size() / 2];
		std::string deleteQuery = "DELETE FROM " + symbol + timeframe + item.source + " where openTime < " + item.openTime + ";";
		int rc = sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, &this->err);
		this->check_rc(rc);
		std::cout << "DELETED " << std::endl;
		// clean vector
		for (int i = 0; i < priceInfoVector.size() - maxNumberOfRows - (maxNumberOfRows / 2); i++) {
			priceInfoVector.erase(priceInfoVector.begin(), priceInfoVector.begin() + 1);
		}
	}
	return;
}

void DBclass::retrieveSymbols()
{
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(dbSmybol, "select symbol, source from symbols", -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		struct infoStruct tempInfoStruct;
		tempInfoStruct.symbol = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		tempInfoStruct.source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		this->symbols.push_back(tempInfoStruct);
	}
	sqlite3_finalize(stmt);
	return;
}