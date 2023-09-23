#include "endpoints.hpp"
#include "symbolSource.h"
#include <iostream>
#include "DBclass.h"
#include "crow.h"
#include "ipLimiter.h"


void exchanges(crow::App<ipLimiter, tokenLimiterStruct, requiredAPIParamsStruct>& app, DBclass& db)
{
	CROW_ROUTE(app, "/exchanges").methods(crow::HTTPMethod::GET)
		.CROW_MIDDLEWARES(app, tokenLimiterStruct)
		([&db]() {
		std::vector<std::string> exchanges = getExchanges(db);
		crow::json::wvalue response;
		if (exchanges.size() == 0) {
			return crow::response(204, "Database failure");
		}
		response["exchanges"] = exchanges;
		return crow::response(response);
	});
}

std::vector<std::string> getExchanges(DBclass &db)
{
	std::vector<std::string> exchanges;
	std::vector<infoStruct> symbols = db.getSymbols();

	for (int i = 0; i < symbols.size(); i++) {
		exchanges.push_back(symbols[i].source);
	}

	return exchanges;
}


