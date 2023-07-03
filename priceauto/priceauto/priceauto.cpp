#include <iostream>
#include "sqlite3.h"
#include <string>
#include "DBclass.h"
#include "scrapeClass.h"
#include "Windows.h"
#include <thread>

int init_scrapeObjVector(int n, DBclass db, std::vector<scrapeClass> &scrapeObjVector) {
	for (int i = 0; i < n; i++) {
		scrapeClass tempScrapeObj;
		tempScrapeObj.init(db.symbols[i].symbol, db.symbols[i].source);
		scrapeObjVector.push_back(tempScrapeObj);
	}
	return 0;
}

class thread_obj {
public:
	void operator()(int x)
	{
		for (int i = 0; i < x; i++)
			std::cout << "Thread using function"
			" object as callable\n";
	}
};

void square() {
	std::cout << "test";
}

int main() {
	DBclass db;
	int n = db.symbols.size();
	std::vector <scrapeClass> scrapeObjVector;

	init_scrapeObjVector(db.symbols.size(), db, scrapeObjVector);


	for (;;) {
		std::vector<std::thread> threads;
		for (int i = 0; i < n; i++) {
			threads.push_back(std::thread(&scrapeClass::scrapeMain, &scrapeObjVector[i]));
		}
		for (int i = 0; i < n; i++) {
			threads[i].join();
		}
		Sleep(5000);
	}
}
