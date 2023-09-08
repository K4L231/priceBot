#include "scrapeClass.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <future>
#include "symbolSource.h"
#include <ctime>
#include <chrono>

void scrapeClass::init(std::string symbol, std::string source, std::string steamHash)
{
	this->symbol = symbol;
	this->source = source;
	this->steamHash = steamHash;
}

void scrapeClass::scrapeMain(std::promise<infoStruct>&& p)
{
	if (source == "binance") {
		this->scrapeBinance();
	}

	if (source == "bitstamp") {
		this->scrapeBitstamp();
	}

	if (source == "kraken") {
		this->scrapeKraken();
	}

	if (source == "kucoin") {
		this->scrapeKucoin();
	}

	if (source == "steam") {
		this->scrapeSteam();
	}

	p.set_value(this->priceInfo);
//	std::cout << priceInfo.last << std::endl;
}

void scrapeClass::insertLast(std::promise<infoStruct>&& p, infoStruct retrievedStruct)
{
	retrievedStruct.responseCode = 200;
	p.set_value(retrievedStruct);
}

void scrapeClass::scrapeSteam()
{
	this->priceInfo.source = "steam";
	this->priceInfo.symbol = this->symbol;
	
	auto response = cpr::Get(cpr::Url{"https://steamcommunity.com/market/priceoverview/?country=LT&currency=1&appid=730&market_hash_name=" + this->steamHash}, cpr::Timeout{3000});
	if (response.status_code == 200) {
		nlohmann::json j_complete = nlohmann::json::parse(response.text);

		bool recordPrice = false;
		std::string priceStr = to_string(j_complete["median_price"]);
		std::string priceStrNew;

		for (int i = 0; i < priceStr.size(); i++) {
			if (priceStr[i] == "$"[0]) {
				recordPrice = true;
				continue;
			}
			if (priceStr[i] == " "[0]) break;
			if (recordPrice == true) {
				priceStrNew = priceStrNew + priceStr[i];
			}
		}

		this->priceInfo.open = std::stod(priceStrNew);
		this->priceInfo.high = std::stod(priceStrNew);
		this->priceInfo.low = std::stod(priceStrNew);
		this->priceInfo.volume = std::stod(j_complete["volume"].get<std::string>());
		this->priceInfo.quoteVolume = "0";
		this->priceInfo.last = std::stod(priceStrNew);
		this->priceInfo.change = 0;
		this->priceInfo.prcntChange = 0;

		this->priceInfo.responseCode = 200;
	}
	else {
		this->priceInfo.responseCode = response.status_code;
		std::cout << "steam market response : " << std::endl;
	}
}

void scrapeClass::scrapeKucoin()
{
	this->priceInfo.source = "kucoin";
	this->priceInfo.symbol = this->symbol;

	auto currentTime = std::chrono::system_clock::now();
	std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
	int timestamp = static_cast<int>(currentTimeT) - 120;

	auto response = cpr::Get(cpr::Url{"https://api.kucoin.com/api/v1/market/candles?type=1min&symbol=" + this->symbol}, cpr::Timeout{3000});
	if (response.status_code != 200) {
		this->priceInfo.responseCode = response.status_code;
		std::cout << "Kucoin candles response : " << std::endl;
	}

	auto response1 = cpr::Get(cpr::Url{"https://api.kucoin.com/api/v1/market/orderbook/level1?symbol=" + this->symbol}, cpr::Timeout{3000});
	if (response1.status_code != 200) {
		this->priceInfo.responseCode = response1.status_code;
		std::cout << "Kucoin ticker response : " << std::endl;
	}

	if (response.status_code == 200 && response1.status_code == 200) {
		nlohmann::json j_complete = nlohmann::json::parse(response.text);
		nlohmann::json j_complete1 = nlohmann::json::parse(response1.text);
		nlohmann::json item = j_complete["data"][0];
		this->priceInfo.open = std::stod(item[1].get<std::string>());
		this->priceInfo.high = std::stod(item[3].get<std::string>());
		this->priceInfo.low = std::stod(item[4].get<std::string>());
		this->priceInfo.volume = std::stod(item[5].get<std::string>());
		this->priceInfo.quoteVolume = item[6].get<std::string>();
		this->priceInfo.last = std::stod(j_complete1["data"]["price"].get<std::string>());
		this->priceInfo.change = 0;
		this->priceInfo.prcntChange = 0;

		this->priceInfo.responseCode = 200;
	}

}

void scrapeClass::scrapeKraken()
{
	this->priceInfo.source = "kraken";
	this->priceInfo.symbol = this->symbol;

	auto response = cpr::Get(cpr::Url{"https://api.kraken.com/0/public/OHLC?pair=" + this->symbol + "&interval=1"}, cpr::Timeout{3000});
	if (response.status_code != 200) {
		this->priceInfo.responseCode = response.status_code;
		std::cout << "KRAKEN OHLC response : " << std::endl;
	}

	auto response1 = cpr::Get(cpr::Url{"https://api.kraken.com/0/public/Ticker?pair=" + this->symbol}, cpr::Timeout{3000});
	if (response1.status_code != 200) {
		this->priceInfo.responseCode = response1.status_code;
		std::cout << "KRAKEN TCIKER response : " << std::endl;
	}

	if (response.status_code == 200 && response1.status_code == 200) {
		nlohmann::json j_complete = nlohmann::json::parse(response.text);
		nlohmann::json j_complete1 = nlohmann::json::parse(response1.text);
		std::string symbolString = to_string(j_complete["result"]);
		bool recordSymbol = false;
		std::string symbol = "";

		if (symbolString.size() < 15) {
			this->priceInfo.responseCode = 600;
		}
		else {
			for (int i = 0; i < 15; i++) {
				if (symbolString[i] == "\""[0]) {
					recordSymbol = !recordSymbol;
					continue;
				}
				if (recordSymbol == true) {
					symbol = symbol + symbolString[i];
				}
			}
			nlohmann::json item = j_complete["result"][symbol][j_complete["result"][symbol].size() - 1];
			this->priceInfo.open = std::stod(item[1].get<std::string>());
			this->priceInfo.high = std::stod(item[2].get<std::string>());
			this->priceInfo.low = std::stod(item[3].get<std::string>());
			this->priceInfo.volume = std::stod(item[6].get<std::string>());
			this->priceInfo.last = std::stod(j_complete1["result"][symbol]["a"][0].get<std::string>());
			this->priceInfo.quoteVolume = "0";
			this->priceInfo.change = 0;
			this->priceInfo.prcntChange = 0;
			this->priceInfo.responseCode = 200;
		}
	}
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