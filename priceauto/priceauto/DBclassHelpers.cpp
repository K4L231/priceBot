#include "DBclass.h"
#include <iostream>
#include <string>
#include "sqlite3.h"

void DBclass::check_rc(int rc)
{
	if (rc != SQLITE_OK) {
		std::cout << "err: " << err;
	}
}

std::string DBclass::quotesql(const std::string& s)
{
	return std::string("'") + s + std::string("'");
}