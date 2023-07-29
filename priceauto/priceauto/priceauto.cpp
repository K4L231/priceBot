#include <iostream>
#include "sqlite3.h"
#include <string>
#include "DBclass.h"
#include "scrapeClass.h"
#include "timeClass.h"
#include "Windows.h"
#include <thread>
#include <future>
#include "symbolSource.h"
#include <chrono>

int init_scrapeObjVector(int n, DBclass db, std::vector<scrapeClass> &scrapeObjVector) {
	for (int i = 0; i < n; i++) {
		scrapeClass tempScrapeObj;
		tempScrapeObj.init(db.symbols[i].symbol, db.symbols[i].source);
		scrapeObjVector.push_back(tempScrapeObj);
	}
	return 0;
}


int main() {
	timeClass getTime;
	DBclass db;
	int n = db.symbols.size();
	std::vector <scrapeClass> scrapeObjVector;
	init_scrapeObjVector(db.symbols.size(), db, scrapeObjVector);

	int sleep = 5000;
	int minute = -1;

	for (;;) {
		time_t currentTime = time(nullptr);
		struct tm localTimeInfo;
		localtime_s(&localTimeInfo, &currentTime);
		int tempMinute = localTimeInfo.tm_min;


		auto start = std::chrono::high_resolution_clock::now();
		std::vector<std::thread> ScrapeThreads;
		std::vector<std::future<infoStruct>> futureResults;

		for (int i = 0; i < n; i++) {
			std::promise<infoStruct> promise;
			futureResults.push_back(promise.get_future());
			ScrapeThreads.push_back(std::thread(&scrapeClass::scrapeMain, &scrapeObjVector[i], std::move(promise)));
		}
		for (int i = 0; i < n; i++) {
			ScrapeThreads[i].join();
			auto fval = futureResults[i].get();
			fval.openTime = std::to_string(localTimeInfo.tm_year) + std::to_string(localTimeInfo.tm_mon) + std::to_string(localTimeInfo.tm_mday) + std::to_string(localTimeInfo.tm_hour) + std::to_string(tempMinute);
			db.insertInterval(fval, n, "interval");
		}


		if (tempMinute != minute) {
			minute = tempMinute;
			std::vector<std::thread> dbIntervalThreads;
			for (int i = 0; i < n; i++) {
				dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "1min", db.symbols[i].symbol, "interval", 60000 / sleep));
			}
			if (minute % 3 == 0) {
				for (int i = 0; i < n; i++) {
					dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "3min", db.symbols[i].symbol, "interval", (60000 * 3) / sleep));
				}
			}
			if (minute % 5 == 0) {
				for (int i = 0; i < n; i++) {
					dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "5min", db.symbols[i].symbol, "interval", (60000 * 5) / sleep));
				}
			}
			if (minute % 15 == 0) {
				for (int i = 0; i < n; i++) {
					dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "15min", db.symbols[i].symbol, "interval", (60000 * 15) / sleep));
				}
			}
			if (minute % 30 == 0) {
				for (int i = 0; i < n; i++) {
					dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "30min", db.symbols[i].symbol, "interval", (60000 * 30) / sleep));
				}
			}


			for (int i = 0; i < dbIntervalThreads.size(); i++) {
				dbIntervalThreads[i].join();
			}
		}







		auto end = std::chrono::high_resolution_clock::now();
		int time = (end.time_since_epoch().count() - start.time_since_epoch().count()) / 1000000;
		std::cout << "Took miliseconds to execute: " << time << std::endl;
		if (sleep - time > 0) Sleep(sleep - time);
		
	}
}
