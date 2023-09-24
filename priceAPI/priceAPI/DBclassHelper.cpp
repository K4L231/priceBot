#include "DBclass.h"
#include <iostream>
#include <string>
#include "sqlite3.h"

int DBclass::check_rc(int rc)
{
	if (rc != 0) {
		std::cout << "err: " + rc << "SQL ERROR" << std::endl;
		return 0;
	}

	return 0;
}

int DBclass::convertOpenTimeToMin(std::string openTime)
{
	if (openTime.size() < 12) return 0;
	char test = openTime[9];
	char test1 = openTime[10];
	std::string rawOpenTime = std::to_string((test - '0') * 10 + (test1 - '0'));
//	std::cout << "RAW TIME: " << (test - '0') * 10 + (test1 - '0') << std::endl;
	return std::stoi(rawOpenTime);
}

int DBclass::convertOpenTimeToHour(std::string openTime)
{
	if (openTime.size() < 12) return 0;
	char test = openTime[7];
	char test1 = openTime[8];
	std::string rawOpenTime = std::to_string((test - '0') * 10 + (test1 - '0'));
	//	std::cout << "RAW TIME: " << (test - '0') * 10 + (test1 - '0') << std::endl;
	return std::stoi(rawOpenTime);
}
