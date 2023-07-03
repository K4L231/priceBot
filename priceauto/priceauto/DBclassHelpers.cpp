#include "DBclass.h"
#include <iostream>
#include <string>
#include "sqlite3.h"

int DBclass::check_rc(int rc)
{
	if (rc != SQLITE_OK) {
		std::cout << "err: " << err;
		return 8;
	}

	return 0;
}

std::string DBclass::quotesql(const std::string& s)
{
	return std::string("'") + s + std::string("'");
}