#include "endpoints.hpp"
#include <crow.h>
#include "tokenLimiterStruct.h"
#include "symbolSource.h"
#include "requiredAPIParams.h"


void priceData(crow::App<ipLimiter, tokenLimiterStruct, requiredAPIParamsStruct>& app, DBclass& db)
{
    CROW_ROUTE(app, "/priceData")
        .methods(crow::HTTPMethod::GET)
        .CROW_MIDDLEWARES(app, tokenLimiterStruct, requiredAPIParamsStruct)
        ([&](const crow::request& req) {

        std::vector<infoStruct> priceData;
        
        if (req.url_params.get("limit") == nullptr || req.url_params.get("limit") == "1") {
            priceData = db.priceDataRetrieve(req.url_params.get("symbol"), req.url_params.get("interval"), req.url_params.get("exchange"), 1);
        }
        else {
            std::string limitString = "";
            std::string rawLimitString = req.url_params.get("limit");
            for (char c : rawLimitString) {
                if (!std::isalpha(c)) {
                    limitString += c;
                }
            }
            if (limitString.size() == 0) {
                return crow::response(400, "Invalid limit parameter");
            }
            int limit = std::stoi(limitString);
            priceData = db.priceDataRetrieve(req.url_params.get("symbol"), req.url_params.get("interval"), req.url_params.get("exchange"), limit);
        }

        std::vector<std::vector<std::string>> responseVector;
        for (int i = 0; i < priceData.size(); i++) {
//            std::vector<std::string> singlePriceInfo = { priceData[i].source, priceData[i].openTime, priceData[i].open, priceData[i].last };
            responseVector.push_back({ priceData[i].openTime, std::to_string(priceData[i].open), std::to_string(priceData[i].last), std::to_string(priceData[i].high), std::to_string(priceData[i].low), std::to_string(priceData[i].volume), priceData[i].quoteVolume});
        }

        crow::json::wvalue response;
        response["symbols"] = responseVector;
        return crow::response(response);
    });
}