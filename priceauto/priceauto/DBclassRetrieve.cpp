#include "DBclass.h"

void DBclass::retrievePriceInfo(std::string symbol, std::string timeframe, std::vector<infoStruct>& priceInfoVector)
{
	std::string sqlQuery = "select symbol, change, prcntChange, high, low, last, open, volume, count from " + symbol + timeframe;
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, 0);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		struct infoStruct tempInfoStruct;
		tempInfoStruct.symbol	   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		tempInfoStruct.change      = sqlite3_column_double(stmt, 1);
		tempInfoStruct.prcntChange = sqlite3_column_double(stmt, 2);
		tempInfoStruct.high        = sqlite3_column_double(stmt, 3);
		tempInfoStruct.low         = sqlite3_column_double(stmt, 4);
		tempInfoStruct.last        = sqlite3_column_double(stmt, 5);
		tempInfoStruct.open        = sqlite3_column_double(stmt, 6);
		tempInfoStruct.volume      = sqlite3_column_double(stmt, 7); 
		tempInfoStruct.count       = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
		priceInfoVector.push_back(tempInfoStruct);
	}
	this->cleanUpData(priceInfoVector, symbol, timeframe);
}

void DBclass::cleanUpData(std::vector<infoStruct>& priceInfoVector, std::string symbol, std::string timeframe) {
	int maxNumberOfRows = 15;
	if (priceInfoVector.size() > maxNumberOfRows) {
		for (int i = 0; i < priceInfoVector.size() - maxNumberOfRows; i++) {
			infoStruct item = priceInfoVector[i];
			std::string deleteQuery = "DELETE FROM " + symbol + timeframe + " where " +
				"symbol = " + "\'" + item.symbol + "\'" +
				" AND change = " + std::to_string(item.change) +
				" AND prcntChange = " + std::to_string(item.prcntChange) +
				" AND high = " + std::to_string(item.high) +
				" AND low = " + std::to_string(item.low) +
				" AND last = " + std::to_string(item.last) +
				" AND open = " + std::to_string(item.open) +
				" AND volume = " + std::to_string(item.volume) +
				" AND count = " + "\'" + item.count + "\'"
				+ ";";
			this->rc = sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, &this->err);
			this->check_rc(this->rc);
		}
		// clean vector
		for (int i = 0; i < priceInfoVector.size() - maxNumberOfRows; i++) {
			priceInfoVector.erase(priceInfoVector.begin(), priceInfoVector.begin() + 1);
		}
	}
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
}