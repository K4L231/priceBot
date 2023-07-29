#include "scrapeClass.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <future>
#include "symbolSource.h"
void scrapeClass::init(std::string symbol, std::string source)
{
	this->symbol = symbol;
	this->source = source;

}

void scrapeClass::scrapeMain(std::promise<infoStruct>&& p)
{
	if (source == "binance") {
		this->scrapeBinance();
	}

	p.set_value(this->priceInfo);
//	std::cout << priceInfo.last << std::endl;
}

void scrapeClass::scrapeBinance()
{
	auto response = cpr::Get(cpr::Url{"https://api.binance.com/api/v3/ticker?symbol=" + this->symbol + "&windowSize=1m"});

	nlohmann::json j_complete = nlohmann::json::parse(response.text);
	this->priceInfo.symbol = this->symbol;
	this->priceInfo.change = std::stod(j_complete["priceChange"].get<std::string>());
	this->priceInfo.prcntChange = std::stod(j_complete["priceChangePercent"].get<std::string>());
	this->priceInfo.high   = std::stod(j_complete["highPrice"].get<std::string>());
	this->priceInfo.low    = std::stod(j_complete["lowPrice"].get<std::string>());
	this->priceInfo.last   = std::stod(j_complete["lastPrice"].get<std::string>());
	this->priceInfo.open   = std::stod(j_complete["openPrice"].get<std::string>());
	this->priceInfo.volume = std::stod(j_complete["volume"].get<std::string>());
	this->priceInfo.quoteVolume = j_complete["quoteVolume"].get<std::string>();
	this->priceInfo.openTime = std::to_string(j_complete["openTime"].get<long long>());

}



