#include "DBclass.h"
#include "symbolSource.h"

void DBclass::insertTimeframe(std::string timeframe, std::string symbol, std::string smallerTimeframe, int numberOfRows, bool temp, std::string source)
{
	std::cout << timeframe << " " << symbol << std::endl;
	const std::string create_table = "CREATE TABLE IF NOT EXISTS " + symbol + timeframe + source + " (symbol text, change numeric, prcntChange numeric, high numeric, low numeric, last numeric, open numeric, volume numeric, quoteVolume text, openTime string)";
	int rc = sqlite3_exec(db, create_table.c_str(), NULL, NULL, &this->err);
	this->check_rc(rc);

	std::vector <infoStruct> priceInfoVector;
	this->retrievePriceInfo(symbol, smallerTimeframe, priceInfoVector, source);

	if (numberOfRows < priceInfoVector.size()) {
		infoStruct tempPriceStruct;
		tempPriceStruct.symbol = priceInfoVector[0].symbol;
//		tempPriceStruct.change = priceInfoVector[priceInfoVector.size() - 1].last - priceInfoVector[priceInfoVector.size() - numberOfRows - 1].open;
//		tempPriceStruct.prcntChange = (tempPriceStruct.change * 100) / priceInfoVector[priceInfoVector.size() - 1].last;
		tempPriceStruct.low = 99999999999999999;
		tempPriceStruct.volume = 0;
		long long tempQuoteVolume = 0;
		for (int i = priceInfoVector.size() - numberOfRows - 1; i < priceInfoVector.size() - 1; i++) {
			if (priceInfoVector[i].high > tempPriceStruct.high) {
				tempPriceStruct.high = priceInfoVector[i].high;
			}
			if (priceInfoVector[i].low < tempPriceStruct.low) {
				tempPriceStruct.low = priceInfoVector[i].low;
			}
		}

		tempPriceStruct.volume = priceInfoVector[priceInfoVector.size() - 1].volume;
		tempQuoteVolume = std::stoll(priceInfoVector[priceInfoVector.size() - 1].quoteVolume);
		for (int i = priceInfoVector.size() - 1; i > priceInfoVector.size() - numberOfRows; i--) {
			if (priceInfoVector[i].openTime != priceInfoVector[i - 1].openTime) {
				tempPriceStruct.volume = tempPriceStruct.volume + priceInfoVector[i - 1].volume;
				tempQuoteVolume = tempQuoteVolume + std::stoll(priceInfoVector[i - 1].quoteVolume);
			}
		}

		tempPriceStruct.last = priceInfoVector[priceInfoVector.size() - 1].last;
		tempPriceStruct.quoteVolume = std::to_string(tempQuoteVolume);
		tempPriceStruct.open = priceInfoVector[priceInfoVector.size() - numberOfRows - 1].last;
		tempPriceStruct.openTime = priceInfoVector[priceInfoVector.size() - numberOfRows - 1].openTime;
		tempPriceStruct.change = tempPriceStruct.last - tempPriceStruct.open;
		tempPriceStruct.prcntChange = (tempPriceStruct.change * 100) / tempPriceStruct.last;

		if (temp == false && timeframe != "interval") {
			this->deleteLastRow(tempPriceStruct.symbol, timeframe, source);
			this->retrievePriceInfo(symbol, timeframe, priceInfoVector, source);

			this->insertInterval(tempPriceStruct, this->checked, timeframe, source);

			tempPriceStruct.openTime = std::to_string(std::stoll(tempPriceStruct.openTime) + 1);
			tempPriceStruct.change = 0;
			tempPriceStruct.prcntChange = 0;
			tempPriceStruct.high = tempPriceStruct.last;
			tempPriceStruct.low = tempPriceStruct.last;
			tempPriceStruct.open = tempPriceStruct.last;
			tempPriceStruct.volume = 0;
			tempPriceStruct.quoteVolume = "0";
			this->insertInterval(tempPriceStruct, this->checked, timeframe, source);
		}
		else if (temp == true && timeframe != "interval") {
//			tempPriceStruct.quoteVolume = priceInfoVector[priceInfoVector.size() - 1].quoteVolume;
//			tempPriceStruct.volume = priceInfoVector[priceInfoVector.size() - 1].volume;
			this->deleteLastRow(tempPriceStruct.symbol, timeframe, source);
			this->insertInterval(tempPriceStruct, this->checked, timeframe, source);
		}
	}

}

void DBclass::insertInterval(infoStruct priceInfo, int n, std::string timeframe, std::string source)
{
	if (this->checked < n) {
		this - checked++;
		const std::string create_table = "CREATE TABLE IF NOT EXISTS " + priceInfo.symbol + timeframe + source + " (symbol text, change numeric, prcntChange numeric, high numeric, low numeric, last numeric, open numeric, volume numeric, quoteVolume text, openTime string)";
		int rc = sqlite3_exec(db, create_table.c_str(), NULL, NULL, &this->err);
		this->check_rc(rc);
	}
	std::string sql("INSERT INTO " + priceInfo.symbol + timeframe + source + " (symbol, change, prcntChange, high, low, last, open, volume, quoteVolume, openTime) VALUES ("
		+ this->quotesql(priceInfo.symbol) + ","
		+ this->quotesql(std::to_string(priceInfo.change)) + ","
		+ this->quotesql(std::to_string(priceInfo.prcntChange)) + ","
		+ this->quotesql(std::to_string(priceInfo.high)) + ","
		+ this->quotesql(std::to_string(priceInfo.low)) + ","
		+ this->quotesql(std::to_string(priceInfo.last)) + ","
		+ this->quotesql(std::to_string(priceInfo.open)) + ","
		+ this->quotesql(std::to_string(priceInfo.volume)) + ","
		+ this->quotesql(priceInfo.quoteVolume) + ","
		+ this->quotesql(priceInfo.openTime) + ");");

	int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &this->err);
	this->check_rc(rc);
}

void DBclass::insertToSymbols(std::string symbol, std::string source)
{
	std::string sql("INSERT INTO symbols (symbol, source) VALUES ("
		+ this->quotesql(symbol) + ","
		+ this->quotesql(source) + ");");

	this->rc = sqlite3_exec(dbSmybol, sql.c_str(), NULL, 0, &this->err);

	if (this->check_rc(this->rc) == 0) {
		std::cout << "inserted symbol - source: " << symbol << " - " << source << std::endl;
	}
}