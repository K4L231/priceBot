#include "endpoints.hpp"
#include "symbolSource.h"
#include <iostream>
#include "DBclass.h"
#include "crow.h"

void symbols(crow::App<ipLimiter, tokenLimiterStruct, requiredAPIParamsStruct>& app, DBclass& db)
{
    CROW_ROUTE(app, "/symbols")
    .methods(crow::HTTPMethod::GET)
    .CROW_MIDDLEWARES(app, tokenLimiterStruct)
    ([&](const crow::request& req) {

        std::vector<std::vector<std::string>> symbols;
        std::vector<infoStruct> symbolsData = db.getSymbols();
        crow::json::wvalue response;
        
        if (req.url_params.get("exchange") != nullptr) {
            std::string exchange = req.url_params.get("exchange");
            for (int i = 0; i < symbolsData.size(); i++) {
                if (symbolsData[i].source == exchange) {
                    std::vector<std::string> singleSymbolData = { symbolsData[i].symbol , symbolsData[i].source };
                    symbols.push_back(singleSymbolData);
                }
            }
            response["symbols"] = symbols;
            return crow::response(response);
        }
        
        for (int i = 0; i < symbolsData.size(); i++) {
            std::vector<std::string> singleSymbolData = { symbolsData[i].symbol , symbolsData[i].source };
            symbols.push_back(singleSymbolData);
        }
        
        response["symbols"] = symbols;
        return crow::response(response);
    });
}

