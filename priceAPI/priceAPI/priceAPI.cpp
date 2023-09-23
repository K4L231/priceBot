#include <iostream>
#include "crow.h"
#include "DBclass.h"
#include "endpoints.hpp"
#include "ipLimiterStruct.h"
#include "tokenLimiterStruct.h"
#include "requiredAPIParams.h"

int main()
{

    DBclass db;
    crow::App <ipLimiter, tokenLimiterStruct, requiredAPIParamsStruct> app;
    app.get_middleware<tokenLimiterStruct>().setDB(db);
    app.get_middleware<ipLimiter>().setipLimit(15);

    exchanges(app, db);
    symbols(app, db);
    priceData(app, db);
    
    app.port(18080).multithreaded().run();
}
