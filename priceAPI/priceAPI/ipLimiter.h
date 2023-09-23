#pragma once

class ipLimiterClass {
private:
	int requestLimit;
public:
	ipLimiterClass(int limit);

	void ipLimit_check(crow::request req, crow::response& res);

};