#include "sqlite3.h"
#include "symbolSource.h"
#include <vector>
#include "userToken.h"
#pragma once
class DBclass {
public:
	sqlite3* db;
	sqlite3* dbSmybol;
	DBclass();
	std::vector<infoStruct> getSymbols();
	infoStruct validateSymbol(std::string symbol, std::string exchange);
	userToken validateToken(std::string token);
	std::vector<infoStruct> priceDataRetrieve(std::string symbol, std::string timeframe, std::string source, int limit);
private:
	int check_rc(int rc);
};