#include "scrapeClass.h"
#include <cpr/cpr.h>

void scrapeClass::init(std::string symbol, std::string source)
{
	this->symbol = symbol;
	this->source = source;
}

void scrapeClass::print() {
	std::cout << this->symbol << std::endl;
}

void scrapeClass::scrapeMain()
{
	if (source == "binance") {
		this->scrapeBinance();
	}

}

void scrapeClass::scrapeBinance()
{
	std::string symbol = "BTCUSDT";
	auto response = cpr::Get(cpr::Url{"https://api.binance.com/api/v3/ticker?symbol=" + this->symbol + "&windowSize=1m"});
	std::cout << response.text << std::endl;
}
