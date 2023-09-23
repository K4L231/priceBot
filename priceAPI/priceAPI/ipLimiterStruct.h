#pragma once
#pragma warning(disable : 4996)
#include <string>
#include <iostream>
#include <crow.h>
#include <ctime>

struct ipLimiter
{
    int limitCount;
    std::map<std::string, int> ipMap;

    int currentMinute;
    std::tm calendar_time;


    struct context
    {
    };

    void setipLimit(int limit)
    {
        limitCount = limit;
    }

    void before_handle(crow::request& req/*req*/, crow::response& res/*res*/, context& /*ctx*/)
    {
        const std::time_t now = std::time(nullptr);
        calendar_time = *std::localtime(std::addressof(now));

        if (currentMinute != calendar_time.tm_min) {
            currentMinute = calendar_time.tm_min;
            ipMap.clear();
        }

        if (ipMap.find(req.remote_ip_address) == ipMap.end()) {
            ipMap[req.remote_ip_address] = 1;
        }
        else {
            if (ipMap[req.remote_ip_address] > limitCount) {
                res.code = 429;
                res.write("Too many requests");
                res.end();
            }
            ipMap[req.remote_ip_address]++;
        }
    }
    void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/)
    {
        // no-op
    }
};

