#include "DBclass.h"
#include <iostream>
#include <string>
#include "sqlite3.h"

int DBclass::check_rc(int rc)
{
	if (rc != SQLITE_OK) {
		std::cout << "err: " << sqlite3_errstr(rc) << std::endl;
		return 0;
	}

	return 0;
}