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

	if (source == "bitstamp") {
		this->scrapeBitstamp();
	}
	if (source == "binanceFutures") {
		this->scrapeBinanceFutures();
	}
	p.set_value(this->priceInfo);
//	std::cout << priceInfo.last << std::endl;
}


//NOT WORKING
void scrapeClass::scrapeBinanceFutures()
{

	auto response1 = cpr::Get(cpr::Url{"https://binance.com/fapi/v1/ticker/24hr?symbol=" + this->symbol}, cpr::Timeout{1000});
	this->priceInfo.responseCode = response1.status_code;
	this->priceInfo.symbol = this->symbol;

	if (response1.status_code == 200) {
		nlohmann::json j_complete1 = nlohmann::json::parse(response1.text);
		this->priceInfo.change = std::stod(j_complete1["priceChange"].get<std::string>());
		this->priceInfo.prcntChange = std::stod(j_complete1["priceChangePercent"].get<std::string>());
		this->priceInfo.high = std::stod(j_complete1["highPrice"].get<std::string>());
		this->priceInfo.low = std::stod(j_complete1["lowPrice"].get<std::string>());
		this->priceInfo.last = std::stod(j_complete1["lastPrice"].get<std::string>());
		this->priceInfo.open = std::stod(j_complete1["openPrice"].get<std::string>());

		this->priceInfo.volume = std::stod(j_complete1["volume"].get<std::string>());
		this->priceInfo.quoteVolume = j_complete1["quoteVolume"].get<std::string>();
		//	this->priceInfo.openTime = std::to_string(j_complete[0][0].get<long long>());
	}
	this->priceInfo.source = "binanceFutures";
}

void scrapeClass::scrapeBitstamp()
{
	std::string my_str = this->symbol;
	my_str.erase(remove(my_str.begin(), my_str.end(), '/'), my_str.end());
	this->priceInfo.symbol = my_str;
	std::string symbolis = my_str;
	std::transform(symbolis.begin(), symbolis.end(), symbolis.begin(),
		[](unsigned char c) { return std::tolower(c); });

	auto response = cpr::Get(cpr::Url{"https://www.bitstamp.net/api/v2/ohlc/" + symbolis + "/?step=60&limit=1"}, cpr::Timeout{3000});
	auto response1 = cpr::Get(cpr::Url{"https://www.bitstamp.net/api/v2/ticker/" + symbolis + "/"}, cpr::Timeout{3000});

	if (response.status_code != 200) {
		this->priceInfo.responseCode = response.status_code;
		std::cout << "BITSTAMP response : " << std::endl;
	}
	if (response1.status_code != 200) {
		this->priceInfo.responseCode = response1.status_code;
		std::cout << "BITSTAMP response1 : " << std::endl;
	}

	if (response.status_code == 200 && response1.status_code == 200) {
		nlohmann::json j_complete = nlohmann::json::parse(response.text);
		nlohmann::json item = j_complete["data"]["ohlc"][0];

		nlohmann::json j_complete1 = nlohmann::json::parse(response1.text);

		this->priceInfo.change = 0;
		this->priceInfo.prcntChange = 0;
		this->priceInfo.high = std::stod(item["high"].get<std::string>());
		this->priceInfo.low = std::stod(item["low"].get<std::string>());
		this->priceInfo.last = std::stod(j_complete1["last"].get<std::string>());
		this->priceInfo.open = std::stod(item["open"].get<std::string>());
		this->priceInfo.volume = std::stod(item["volume"].get<std::string>());
		this->priceInfo.quoteVolume = "0";
		this->priceInfo.openTime = item["timestamp"].get<std::string>();
		this->priceInfo.responseCode = response.status_code;
	}
	this->priceInfo.source = "bitstamp";
}

void scrapeClass::scrapeBinance()
{
	auto response = cpr::Get(cpr::Url{"https://api.binance.com/api/v3/ticker?symbol=" + this->symbol + "&windowSize=1m"}, cpr::Timeout{3000});
	this->priceInfo.responseCode = response.status_code;

	if (response.error) {
		priceInfo.responseCode = 999;
	}

	if (response.status_code == 200) {
		nlohmann::json j_complete = nlohmann::json::parse(response.text);
		this->priceInfo.change = std::stod(j_complete["priceChange"].get<std::string>());
		this->priceInfo.prcntChange = std::stod(j_complete["priceChangePercent"].get<std::string>());
		this->priceInfo.high = std::stod(j_complete["highPrice"].get<std::string>());
		this->priceInfo.low = std::stod(j_complete["lowPrice"].get<std::string>());
		this->priceInfo.last = std::stod(j_complete["lastPrice"].get<std::string>());
		this->priceInfo.open = std::stod(j_complete["openPrice"].get<std::string>());
		this->priceInfo.volume = std::stod(j_complete["volume"].get<std::string>());
		this->priceInfo.quoteVolume = j_complete["quoteVolume"].get<std::string>();
		this->priceInfo.openTime = std::to_string(j_complete["openTime"].get<long long>());
	}
	this->priceInfo.symbol = this->symbol;
	this->priceInfo.source = "binance";
}


