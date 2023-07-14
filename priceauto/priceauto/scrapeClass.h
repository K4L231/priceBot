#include <iostream>
#include <string>
#include <future>
#include "symbolSource.h"
class scrapeClass {
private:
	void scrapeBinance();
	std::string symbol;
	std::string source;
	infoStruct priceInfo;
	
public:
	void init(std::string symbol, std::string source);
	void scrapeMain(std::promise<infoStruct>&& p);
};