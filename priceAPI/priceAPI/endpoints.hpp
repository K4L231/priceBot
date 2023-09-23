#include "symbolSource.h"
#include <vector>
#include "DBclass.h"
#include "crow.h"
#include "ipLimiterStruct.h"
#include "tokenLimiterStruct.h"
#include "requiredAPIParams.h"
#ifndef ENDPOINTS_HPP
#define ENDPOINTS_HPP


void exchanges(crow::App<ipLimiter, tokenLimiterStruct, requiredAPIParamsStruct>& app, DBclass& db);
std::vector<std::string> getExchanges(DBclass& db);

void symbols(crow::App<ipLimiter, tokenLimiterStruct, requiredAPIParamsStruct>& app, DBclass& db);

void priceData(crow::App<ipLimiter, tokenLimiterStruct, requiredAPIParamsStruct>& app, DBclass& db);

#endif