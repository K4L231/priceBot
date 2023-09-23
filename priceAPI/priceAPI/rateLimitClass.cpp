#include "rateLimitClass.h"

rateLimitClass::rateLimitClass()
{
	this->maxRequestsPerIP = 10;
	this->minute = -1;
}

void rateLimitClass::rateLimitMiddleware()
{
	std::cout << "middleware";
}
