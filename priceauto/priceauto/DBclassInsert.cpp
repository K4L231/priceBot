#include "DBclass.h"

void DBclass::insert()
{
	std::string symbol = "ETHUSDT";
	double test = 5.666;

	std::string sql("INSERT INTO min (symbol, change, high, low, last, open, volume) VALUES ("
		+ this->quotesql("BTCUSDT") + ","
		+ this->quotesql(std::to_string(test)) + ","
		+ this->quotesql("2") + ","
		+ this->quotesql("2") + ","
		+ this->quotesql("2") + ","
		+ this->quotesql("2") + ","
		+ this->quotesql("1") + ");");

	this->rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &this->err);
	this->check_rc(this->rc);
}

void DBclass::insertToSymbols(std::string symbol, std::string source)
{
	std::string sql("INSERT INTO symbols (symbol, source) VALUES ("
		+ this->quotesql(symbol) + ","
		+ this->quotesql(source) + ");");

	this->rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &this->err);

	if (this->check_rc(this->rc) == 0) {
		std::cout << "inserted symbol - source: " << symbol << " - " << source << std::endl;
	}
}