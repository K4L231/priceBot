#include "DBclass.h"
#include <iostream>
#include <string>
#include "sqlite3.h"

int DBclass::check_rc(int rc)
{
//	if (rc != SQLITE_OK) {
//		std::cout << "err: " << err << std::endl;
//		return 8;
//	}

	return 0;
}

std::string DBclass::quotesql(const std::string& s)
{
	return std::string("'") + s + std::string("'");
}

int DBclass::convertOpenTimeToMin(std::string openTime)
{
	std::string rawOpenTime = std::to_string(openTime[9]) + std::to_string(openTime[10]);
	return std::stoi(rawOpenTime);
}