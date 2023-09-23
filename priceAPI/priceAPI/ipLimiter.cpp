#pragma once
#include <crow.h>
#include "ipLimiter.h"

//struct ipLimiter
//{
//

//};

ipLimiterClass::ipLimiterClass(int limit)
{
	this->requestLimit = limit;
}

void ipLimiterClass::ipLimit_check(crow::request req, crow::response& res)
{
    const std::string& clientIP = req.get_header_value("X-Real-IP");
    std::cout << std::to_string(req.get_header_value("Host").find("Host"));
    std::cout << "IP ADRESS::as " + clientIP;
    res.code = 429;
    res.body = "too many reuqests from the IP addres";
    res.end();
}


