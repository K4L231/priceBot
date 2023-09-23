#pragma once
#include "crow.h"
#include "DBclass.h"

struct requiredAPIParamsStruct : crow::ILocalMiddleware
{
    DBclass db;
    struct context {};

    std::map<std::string, int> tokenMap;
    int currentMinute = -1;
    std::tm calendar_time;

    void setDB(DBclass& dba)
    {
        db = dba;
    }

    void before_handle(crow::request& req/*req*/, crow::response& res, context& /*ctx*/)
    {
        if (req.url_params.get("symbol") == nullptr) {
            res.code = 400;
            res.write("Query is missing symbol parameter");
            res.end();
            return;
        }
        if (req.url_params.get("exchange") == nullptr) {
            res.code = 400;
            res.write("Query is missing exchange parameter");
            res.end();
            return;
        }
        if (req.url_params.get("interval") == nullptr) {
            res.code = 400;
            res.write("Query is missing interval parameter");
            res.end();
            return;
        }

        infoStruct symbol = db.validateSymbol(req.url_params.get("symbol"), req.url_params.get("exchange"));
        if (symbol.source.empty()) {
            res.code = 204;
            res.write("Symbol and exchange combination was not found");
            res.end();
            return;
        }

        std::vector<std::string> timeframes{ "1min", "3min", "5min", "15min", "30min", "1hr", "2hr", "4hr", "6hr", "8hr", "12hr", "interval"};
        for (int i = 0; i < timeframes.size(); i++) {
            if (timeframes[i] == req.url_params.get("interval")) break;
            if (i == timeframes.size() - 1) {
                res.code = 204;
                res.write("Requested interval not found");
                res.end();
                return;
            }
        
        }
    }

    void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/)
    {}
};