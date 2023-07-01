#include <iostream>
#include "sqlite3.h"
#include <string>
#include "DBclass.h"
#include "scrapeClass.h"
#include <cpr/cpr.h>

//int init_scrapeObj(int n, DBclass db, scrapeBinanaceClass** scrapeObj) {
//	for (int i = 0; i < n; i++) {
//		scrapeObj[i] = new scrapeBinanaceClass;
//		scrapeObj[i]->init(db.binanceSymbols[i]);
//	}
//	return 0;
//}

int init_scrapeObjVector(int n, DBclass db, std::vector<scrapeClass> &scrapeObjVector) {
	for (int i = 0; i < n; i++) {
		scrapeClass tempScrapeObj;
		tempScrapeObj.init(db.symbols[i].symbol, db.symbols[i].source);
		scrapeObjVector.push_back(tempScrapeObj);
	}
	return 0;
}

int main() {
	DBclass db;
	int n = db.symbols.size();
	std::vector <scrapeClass> scrapeObjVector;

	init_scrapeObjVector(db.symbols.size(), db, scrapeObjVector);

	for (int i = 0; i < n; i++) {
		scrapeObjVector[i].print();
	}
	
//	std::string symbol = "BTCUSDT";
//	auto response = cpr::Get(cpr::Url{"https://api.binance.com/api/v3/ticker?symbol=" + symbol + "&windowSize=1m"});
//	std::cout << response.text << std::endl;
}
