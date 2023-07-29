#include "timeClass.h"
#include <chrono>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

timeClass::timeClass()
{
	this->getCurrentTime();

}

uint64_t timeClass::timeSinceEpochMillisec()
{
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void timeClass::getCurrentTime()
{
	this->currentTime = this->timeSinceEpochMillisec();
}



