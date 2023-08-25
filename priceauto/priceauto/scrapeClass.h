#include <iostream>
#include <string>
#include <future>
#include "symbolSource.h"
#include <cpr/cpr.h>
class scrapeClass {
private:
	void scrapeBinance();
	void scrapeBitstamp();
	void scrapeBinanceFutures();
	std::string symbol;
	std::string source;
	infoStruct priceInfo;
	
public:
	void init(std::string symbol, std::string source);
	void scrapeMain(std::promise<infoStruct>&& p);
};