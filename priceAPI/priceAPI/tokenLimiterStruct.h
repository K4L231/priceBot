#pragma once
#include "crow.h"
#include "DBclass.h"

struct tokenLimiterStruct : crow::ILocalMiddleware
{
    DBclass db;
    struct context{};

    std::map<std::string, int> tokenMap;
    int currentMinute = -1;
    std::tm calendar_time;

    void setDB(DBclass& dba)
    {
        db = dba;
    }

    void before_handle(crow::request& req/*req*/, crow::response& res, context& /*ctx*/)
    {
        if (req.url_params.get("secret") == nullptr) {
            res.write("Your query is missing a secret token");
            res.code = 401;
            res.end();
        }
        else {
            userToken validatedUserToken = db.validateToken(req.url_params.get("secret"));
            if (validatedUserToken.exists == false) {
                res.write("Token is invalid!");
                res.code = 403;
                res.end();
            }
            else {
                const std::time_t now = std::time(nullptr);
                calendar_time = *std::localtime(std::addressof(now));
                if (currentMinute != calendar_time.tm_min) {
                    currentMinute = calendar_time.tm_min;
                    tokenMap.clear();
                }
                if (tokenMap.find(req.url_params.get("secret")) == tokenMap.end()) {
                    tokenMap[req.url_params.get("secret")] = 1;
                }
                else {
                    if (tokenMap[req.url_params.get("secret")] > validatedUserToken.limit) {
                        std::cout << "5" << std::endl;
                        res.code = 429;
                        res.write("Token limit reached");
                        res.end();
                    }
                    tokenMap[req.url_params.get("secret")]++;
                }
            }
        }
    }

    void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/)
    {}
};