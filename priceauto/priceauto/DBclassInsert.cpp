#include "DBclass.h"
#include "symbolSource.h"

void DBclass::insertTimeframe(std::string timeframe, std::string symbol, std::string smallerTimeframe, int numberOfRows)
{
//	std::cout << timeframe << " " << symbol << std::endl;
	const std::string create_table = "CREATE TABLE IF NOT EXISTS " + symbol + timeframe + " (symbol text, change numeric, prcntChange numeric, high numeric, low numeric, last numeric, open numeric, volume numeric, count text)";
	this->rc = sqlite3_exec(db, create_table.c_str(), NULL, NULL, &this->err);
	this->check_rc(this->rc);

	std::vector <infoStruct> priceInfoVector;
	this->retrievePriceInfo(symbol, smallerTimeframe, priceInfoVector);
//	std::cout << priceInfoVector[priceInfoVector.size() - 1].count << std::endl;

	if (numberOfRows < priceInfoVector.size()) {
		infoStruct tempPriceStruct;
		tempPriceStruct.symbol = priceInfoVector[0].symbol;
		tempPriceStruct.change = priceInfoVector[priceInfoVector.size() - numberOfRows - 1].open - priceInfoVector[priceInfoVector.size() - 1].last;
		std::cout << " change: " << tempPriceStruct.change;
	}


}

void DBclass::insertInterval(infoStruct priceInfo, int n, std::string timeframe)
{
	if (this->checked < n) {
		this - checked++;
		const std::string create_table = "CREATE TABLE IF NOT EXISTS " + priceInfo.symbol + timeframe + " (symbol text, change numeric, prcntChange numeric, high numeric, low numeric, last numeric, open numeric, volume numeric, count text)";
		this->rc = sqlite3_exec(db, create_table.c_str(), NULL, NULL, &this->err);
		this->check_rc(this->rc);
	}

	std::string sql("INSERT INTO " + priceInfo.symbol + timeframe + " (symbol, change, prcntChange, high, low, last, open, volume, count) VALUES ("
		+ this->quotesql(priceInfo.symbol) + ","
		+ this->quotesql(std::to_string(priceInfo.change)) + ","
		+ this->quotesql(std::to_string(priceInfo.prcntChange)) + ","
		+ this->quotesql(std::to_string(priceInfo.high)) + ","
		+ this->quotesql(std::to_string(priceInfo.low)) + ","
		+ this->quotesql(std::to_string(priceInfo.last)) + ","
		+ this->quotesql(std::to_string(priceInfo.open)) + ","
		+ this->quotesql(std::to_string(priceInfo.volume)) + ","
		+ this->quotesql(priceInfo.count) + ");");

	this->rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &this->err);
	this->check_rc(this->rc);
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