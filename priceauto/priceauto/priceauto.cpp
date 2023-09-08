#include <thread>
#include <iostream>
#include "sqlite3.h"
#include <string>
#include "DBclass.h"
#include "scrapeClass.h"
#include "timeClass.h"
#include "Windows.h"
#include <future>
#include "symbolSource.h"
#include <chrono>
#include <cpr/cpr.h>

int init_scrapeObjVector(int n, DBclass db, std::vector<scrapeClass> &scrapeObjVector) {
	for (int i = 0; i < n; i++) {
		scrapeClass tempScrapeObj;
		tempScrapeObj.init(db.symbols[i].symbol, db.symbols[i].source, db.symbols[i].steamHash);
		scrapeObjVector.push_back(tempScrapeObj);
	}
	return 0;
}

int main() {
	DBclass db;
	int n = db.symbols.size();
	std::vector <scrapeClass> scrapeObjVector;
	init_scrapeObjVector(db.symbols.size(), db, scrapeObjVector);

	int sleep = 5000;
	int minute = -1;
	int minuteCount = 0;
	int minute3Count = 0;
	int minute3CountCount = 0;
	int minute5Count = 0;
	int minute5CountCount = 0;
	int minute15Count = 0;
	int minute15CountCount = 0;
	int minute30Count = 0;
	int minute30CountCount = 0;

	bool scrapeSteam = true;
	for (;;) {
//	for (int i = 0; i < 1; i++) {
		time_t currentTime = std::time(nullptr);
		struct tm localTimeInfo;
		localtime_s(&localTimeInfo, &currentTime);
		int tempMinute = localTimeInfo.tm_min;


		auto start = std::chrono::high_resolution_clock::now();
		std::vector<std::thread> ScrapeThreads;
		std::vector<std::future<infoStruct>> futureResults;


		for (int i = 0; i < n; i++) {
			std::promise<infoStruct> promise;
			futureResults.push_back(promise.get_future());
			if (scrapeObjVector[i].source == "steam") {
				if (scrapeSteam == true) {
					ScrapeThreads.push_back(std::thread(&scrapeClass::scrapeMain, &scrapeObjVector[i], std::move(promise)));
					scrapeSteam = false;
				}
				else {
					infoStruct tempInfoStruct = db.retriveLastRow(scrapeObjVector[i].priceInfo, scrapeObjVector[i].source);
					ScrapeThreads.push_back(std::thread(&scrapeClass::insertLast, &scrapeObjVector[i], std::move(promise), tempInfoStruct));
				}
			}
			else {
				ScrapeThreads.push_back(std::thread(&scrapeClass::scrapeMain, &scrapeObjVector[i], std::move(promise)));
			}
		}

		for (int i = 0; i < n; i++) {
			
			ScrapeThreads[i].join();

			auto fval = futureResults[i].get();

			fval.openTime = std::to_string(localTimeInfo.tm_year);
			if (std::to_string(localTimeInfo.tm_mon).size() == 1) {
				fval.openTime = fval.openTime + "0" + std::to_string(localTimeInfo.tm_mon);
			}
			else {
				fval.openTime = fval.openTime + std::to_string(localTimeInfo.tm_mon);
			}
			if (std::to_string(localTimeInfo.tm_mday).size() == 1) {
				fval.openTime = fval.openTime + "0" + std::to_string(localTimeInfo.tm_mday);
			}
			else {
				fval.openTime = fval.openTime + std::to_string(localTimeInfo.tm_mday);
			}
			if (std::to_string(localTimeInfo.tm_hour).size() == 1) {
				fval.openTime = fval.openTime + "0" + std::to_string(localTimeInfo.tm_hour);
			}
			else {
				fval.openTime = fval.openTime + std::to_string(localTimeInfo.tm_hour);
			}
			if (std::to_string(tempMinute).size() == 1) {
				fval.openTime = fval.openTime + "0" + std::to_string(tempMinute);
			}
			else {
				fval.openTime = fval.openTime + std::to_string(tempMinute);
			}
			if (std::to_string(localTimeInfo.tm_sec).size() == 1) {
				fval.openTime = fval.openTime + "0" + std::to_string(localTimeInfo.tm_sec);
			}
			else {
				fval.openTime = fval.openTime + std::to_string(localTimeInfo.tm_sec);
			}
			if (fval.responseCode == 200) {
				db.insertInterval(fval, n, "interval", fval.source);
			}
			else {
				std::cout << "Fetch error: " << fval.symbol << " " << fval.source << " " << fval.responseCode << std::endl;
			}
		}

		if (tempMinute != minute) {
			scrapeSteam = true;
			minute = tempMinute;
			std::vector<std::thread> dbIntervalThreads;
			for (int i = 0; i < n; i++) {
//				dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "1min", db.symbols[i].symbol, "interval", 60000 / sleep, false, db.symbols[i].source));
				db.insertTimeframe("1min", db.symbols[i].symbol, "interval", 60000 / sleep, false, db.symbols[i].source);
				minuteCount = 0;
			}
			if (minute % 3 == 0) {
				for (int i = 0; i < n; i++) {
//					dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "3min", db.symbols[i].symbol, "interval", (60000 * 3) / sleep, false, db.symbols[i].source));
					db.insertTimeframe("3min", db.symbols[i].symbol, "interval", (60000 * 3) / sleep, false, db.symbols[i].source);
					minute3Count = 0;
				}
			}
			if (minute % 5 == 0) {
				for (int i = 0; i < n; i++) {
//					dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "5min", db.symbols[i].symbol, "interval", (60000 * 5) / sleep, false, db.symbols[i].source));
					db.insertTimeframe("5min", db.symbols[i].symbol, "interval", (60000 * 5) / sleep, false, db.symbols[i].source);
					minute5Count = 0;
				}
			}
			if (minute % 15 == 0) {
				for (int i = 0; i < n; i++) {
//					dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "15min", db.symbols[i].symbol, "interval", (60000 * 15) / sleep, false, db.symbols[i].source));
					db.insertTimeframe("15min", db.symbols[i].symbol, "interval", (60000 * 15) / sleep, false, db.symbols[i].source);
					minute15Count = 0;
				}
			}
			if (minute % 30 == 0) {
				for (int i = 0; i < n; i++) {
//					dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "30min", db.symbols[i].symbol, "interval", (60000 * 30) / sleep, false, db.symbols[i].source));
					db.insertTimeframe("30min", db.symbols[i].symbol, "interval", (60000 * 30) / sleep, false, db.symbols[i].source);
					minute30Count = 0;
				}
			}
			for (int i = 0; i < dbIntervalThreads.size(); i++) {
				dbIntervalThreads[i].join();
			}
			minuteCount = 0;

			minute3CountCount++;
			minute5CountCount++;
			minute15CountCount++;
			minute30CountCount++;
		}
		else {
			for (int i = 0; i < n; i++) {
				db.insertTimeframe("1min", db.symbols[i].symbol, "interval", minuteCount, true, db.symbols[i].source);
				db.insertTimeframe("3min", db.symbols[i].symbol, "interval", minute3Count, true, db.symbols[i].source);
				db.insertTimeframe("5min", db.symbols[i].symbol, "interval", minute5Count, true, db.symbols[i].source);
				db.insertTimeframe("15min", db.symbols[i].symbol, "interval", minute15Count, true, db.symbols[i].source);
				db.insertTimeframe("30min", db.symbols[i].symbol, "interval", minute30Count, true, db.symbols[i].source);
			}
		}

		minuteCount++;
		minute3Count++;
		minute5Count++;
		minute15Count++;
		minute30Count++;




		auto end = std::chrono::high_resolution_clock::now();
		int time = (end.time_since_epoch().count() - start.time_since_epoch().count()) / 1000000;
		std::cout << "Took miliseconds to execute: " << time << std::endl;
		if (sleep - time > 0) Sleep(sleep - time);

	}


	return 0;
}
