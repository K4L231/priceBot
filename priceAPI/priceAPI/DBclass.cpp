#include "DBclass.h"
#include <iostream>

DBclass::DBclass()
{

	int rc = sqlite3_open("../../myDb.db", &db);
	this->check_rc(rc);
	std::cout << rc;
	rc = sqlite3_open("../../DBsymbol.db", &dbSmybol);
	this->check_rc(rc);
	std::cout << rc;
//	{ "1min", "3min", "5min", "15min", "30min", "1hr", "2hr", "4hr", "6hr", "8hr", "12hr", "interval"};
	this->intervalList["1min"] = 1;
	this->intervalList["3min"] = 3;
	this->intervalList["5min"] = 5;
	this->intervalList["15min"] = 15;
	this->intervalList["30min"] = 30;
	this->intervalList["1hr"] = 60;
	this->intervalList["2hr"] = 60 * 2;
	this->intervalList["4hr"] = 60 * 4;
	this->intervalList["6hr"] = 60 * 6;
	this->intervalList["8hr"] = 60 * 8;
	this->intervalList["12hr"] = 60 * 12;

	this->intervalListHour["1hr"] = 1;
	this->intervalListHour["2hr"] = 2;
	this->intervalListHour["4hr"] = 4;
	this->intervalListHour["6hr"] = 6;
	this->intervalListHour["8hr"] = 8;
	this->intervalListHour["12hr"] = 12;
}

