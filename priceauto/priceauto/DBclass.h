#include <iostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include "symbolSource.h"

class DBclass {
private:
	char* err;
	int rc;
	int check_rc(int rc);
	void insertToSymbols(std::string symbol, std::string source);
	void retrieveSymbols();
	void retrievePriceInfo(std::string symbol, std::string timeframe, std::vector<infoStruct>& priceInfoVector);
	void cleanUpData(std::vector<infoStruct>& priceInfoVector, std::string symbol, std::string timeframe);
	int checked{ 0 };
public:
	sqlite3* db;
	sqlite3* dbSmybol;
	DBclass();

	void insertInterval(infoStruct priceInfo, int n, std::string timeframe);
	std::string quotesql(const std::string& s);
	void getSymbols();
	std::vector <struct infoStruct> symbols;
	void insertTimeframe(std::string timeframe, std::string symbol, std::string smallerTimeframe, int numberOfRows);
};