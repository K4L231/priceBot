#include "DBclass.h"
#include "symbolSource.h"

void DBclass::insertTimeframe(std::string timeframe, std::string symbol, std::string smallerTimeframe, int numberOfRows, bool temp, std::string source)
{
	symbol.erase(remove(symbol.begin(), symbol.end(), '/'), symbol.end());
	symbol.erase(remove(symbol.begin(), symbol.end(), '-'), symbol.end());
//	std::cout << timeframe << " " << symbol << std::endl;
	if (this->checked < 20) {
		const std::string create_table = "CREATE TABLE IF NOT EXISTS " + symbol + timeframe + source + " (symbol text, change numeric, prcntChange numeric, high numeric, low numeric, last numeric, open numeric, volume numeric, quoteVolume text, openTime string)";
		int rc = sqlite3_exec(db, create_table.c_str(), NULL, NULL, &this->err);
		this->check_rc(rc);
		this->createTimeframeTable++;
	}

	std::vector <infoStruct> priceInfoVector;
	this->retrievePriceInfo(symbol, smallerTimeframe, priceInfoVector, source, numberOfRows);
	
	if (numberOfRows + 1 < priceInfoVector.size()) {
		infoStruct tempPriceStruct;





//		int Ioffest = 2;
//		if (temp == true) {
//			Ioffest = 1;
//		}
//
//		tempPriceStruct.symbol = priceInfoVector[0].symbol;
//		tempPriceStruct.low = 99999999999999999;
//		tempPriceStruct.volume = 0;
//		long long tempQuoteVolume = 0;
//		for (int i = priceInfoVector.size() - numberOfRows - 1; i < priceInfoVector.size() - Ioffest; i++) {
//			if (priceInfoVector[i].high > tempPriceStruct.high) {
//				tempPriceStruct.high = priceInfoVector[i].high;
//			}
//			if (priceInfoVector[i].low < tempPriceStruct.low) {
//				tempPriceStruct.low = priceInfoVector[i].low;
//			}
//		}
//
//		tempPriceStruct.volume = priceInfoVector[priceInfoVector.size() - Ioffest].volume;
//		tempQuoteVolume = std::stoll(priceInfoVector[priceInfoVector.size() - Ioffest].quoteVolume);
//		int openTimeChangeCount = 1;
//		for (int i = priceInfoVector.size() - Ioffest; i > priceInfoVector.size() - numberOfRows; i--) {
//			int currOpenTime = this->convertOpenTimeToMin(priceInfoVector[i].openTime);
//			int futuOpenTime = this->convertOpenTimeToMin(priceInfoVector[i - 1].openTime);
//			if (currOpenTime != futuOpenTime) {
//				openTimeChangeCount++;
//				tempPriceStruct.volume = tempPriceStruct.volume + priceInfoVector[i - 1].volume;
//				tempQuoteVolume = tempQuoteVolume + std::stoll(priceInfoVector[i - 1].quoteVolume);
//			}
//		}
//
//		if (priceInfoVector[0].source == "steam") {
//			tempPriceStruct.volume = tempPriceStruct.volume / openTimeChangeCount;
//		}
//
//		tempPriceStruct.last = priceInfoVector[priceInfoVector.size() - Ioffest].last;
//		tempPriceStruct.quoteVolume = std::to_string(tempQuoteVolume);
//		tempPriceStruct.open = priceInfoVector[priceInfoVector.size() - numberOfRows].last;
//		tempPriceStruct.openTime = priceInfoVector[priceInfoVector.size() - numberOfRows - 1].openTime;
//		tempPriceStruct.change = tempPriceStruct.last - tempPriceStruct.open;
//		tempPriceStruct.prcntChange = (tempPriceStruct.change * 100) / tempPriceStruct.last;



		int offset = 0;
		if (temp == false) offset = 1;
		if (temp == true) offset = 0;

		tempPriceStruct.symbol = priceInfoVector[0].symbol;
		tempPriceStruct.last = priceInfoVector[offset].last;
		tempPriceStruct.volume = priceInfoVector[offset].volume;
		tempPriceStruct.quoteVolume = priceInfoVector[offset].quoteVolume;
		tempPriceStruct.low = 99999999999999999;
		tempPriceStruct.high = 0;
		for (int i = offset; i < priceInfoVector.size() - 1; i++) {
			int currOpenTime = this->convertOpenTimeToMin(priceInfoVector[i].openTime);
			int futuOpenTime = this->convertOpenTimeToMin(priceInfoVector[i + 1].openTime);
			if (currOpenTime != futuOpenTime) {
				tempPriceStruct.open = priceInfoVector[i + 1].last;
				break;
			}

//			tempPriceStruct.open = priceInfoVector[i].open;
			tempPriceStruct.openTime = priceInfoVector[i].openTime;
			if (priceInfoVector[i].low < tempPriceStruct.low) {
				tempPriceStruct.low = priceInfoVector[i].low;
			}
			if (priceInfoVector[i].high > tempPriceStruct.high) {
				tempPriceStruct.high = priceInfoVector[i].high;
			}
		}
		tempPriceStruct.change = tempPriceStruct.last - tempPriceStruct.open;
		tempPriceStruct.prcntChange = (tempPriceStruct.change * 100) / tempPriceStruct.last;

		if (temp == false && timeframe != "interval") {
			this->deleteLastRow(tempPriceStruct.symbol, timeframe, source);
			this->retrievePriceInfo(symbol, timeframe, priceInfoVector, source, numberOfRows);

			this->insertInterval(tempPriceStruct, this->checked, timeframe, source);

//			tempPriceStruct.openTime = std::to_string(std::stoll(tempPriceStruct.openTime) + 1);
 			tempPriceStruct.openTime = std::to_string(std::stoll(priceInfoVector[0].openTime) + 1);
//			tempPriceStruct.change = 0;
//			tempPriceStruct.prcntChange = 0;
			tempPriceStruct.high = priceInfoVector[0].last;
			tempPriceStruct.low = priceInfoVector[0].last;
			tempPriceStruct.open = priceInfoVector[1].last;
			tempPriceStruct.volume = priceInfoVector[0].volume;
			tempPriceStruct.quoteVolume = priceInfoVector[0].quoteVolume;
			tempPriceStruct.last = priceInfoVector[0].last;
			tempPriceStruct.change = tempPriceStruct.last - tempPriceStruct.open;
			tempPriceStruct.prcntChange = (tempPriceStruct.change * 100) / tempPriceStruct.last;
			this->insertInterval(tempPriceStruct, this->checked, timeframe, source);
		}
		else if (temp == true && timeframe != "interval") {
			this->deleteLastRow(tempPriceStruct.symbol, timeframe, source);

			this->insertInterval(tempPriceStruct, this->checked, timeframe, source);
		}
	}
}

void DBclass::insertInterval(infoStruct priceInfo, int n, std::string timeframe, std::string source)
{
	priceInfo.symbol.erase(remove(priceInfo.symbol.begin(), priceInfo.symbol.end(), '/'), priceInfo.symbol.end());
	priceInfo.symbol.erase(remove(priceInfo.symbol.begin(), priceInfo.symbol.end(), '-'), priceInfo.symbol.end());
	if (this->checked < 20) {
		this->checked++;
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