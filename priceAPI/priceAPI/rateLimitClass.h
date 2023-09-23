#include <vector>
#include "crow.h"
#pragma once
class rateLimitClass {
public:
	rateLimitClass();
	void rateLimitMiddleware();
private:
	int maxRequestsPerIP;
	int minute;
};