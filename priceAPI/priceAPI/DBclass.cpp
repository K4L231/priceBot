#include "DBclass.h"
#include <iostream>

DBclass::DBclass()
{
	int rc = sqlite3_open("../../../myDb.db", &db);
	this->check_rc(rc);
	std::cout << rc;
	rc = sqlite3_open("../../../DBsymbol.db", &dbSmybol);
	this->check_rc(rc);
	std::cout << rc;
}

