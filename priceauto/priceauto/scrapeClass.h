#include <iostream>
#include <string>
#include <future>
#include "symbolSource.h"
#include <cpr/cpr.h>
#include "sqlite3.h"

class scrapeClass {
private:
	void scrapeBinance();
	void scrapeBitstamp();
	void scrapeBinanceFutures();
	void scrapeKraken();
	void scrapeKucoin();
	void scrapeSteam();
	std::string symbol;
	std::string steamHash;
	sqlite3* db;
	
public:
	void init(std::string symbol, std::string source, std::string steamHash);
	void scrapeMain(std::promise<infoStruct>&& p);
	void insertLast(std::promise<infoStruct>&& p, infoStruct retrievedStruct);
	std::string source;
	infoStruct priceInfo;
};