#include <iostream>
#include "sqlite3.h"
#include <string>
#include "DBclass.h"
#include "scrapeClass.h"
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
	DBclass db;
	int n = db.symbols.size();
	std::vector <scrapeClass> scrapeObjVector;
	init_scrapeObjVector(db.symbols.size(), db, scrapeObjVector);

	int sleep = 5000;

	int minute = -1;
	for (;;) {
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
			db.insertInterval(fval, n, "interval");
		}



		time_t currentTime = time(nullptr);
		struct tm localTimeInfo;
		localtime_s(&localTimeInfo, &currentTime);
		int tempMinute = localTimeInfo.tm_min;
//
		if (tempMinute != minute) {
			minute = tempMinute;
			std::vector<std::thread> dbIntervalThreads;
			for (int i = 0; i < n; i++) {
//				dbIntervalThreads.push_back(std::thread(&DBclass::insertTimeframe, &db, "1min", db.symbols[i].symbol, "interval", 60));
				db.insertTimeframe("1min", db.symbols[i].symbol, "interval", 60000 / sleep);
				std::cout << "end --------------" << std::endl;
			}
			for (int i = 0; i < n; i++) {
//				dbIntervalThreads[i].join();
			}

		}





		auto end = std::chrono::high_resolution_clock::now();
		int time = (end.time_since_epoch().count() - start.time_since_epoch().count()) / 1000000;
		std::cout << "Took miliseconds to execute: " << time << std::endl;
		Sleep(sleep - time);
	}
}
