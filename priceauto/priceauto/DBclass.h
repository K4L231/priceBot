#include <iostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include "symbolSource.h"

class DBclass {
private:
	char* err;
	int rc;
	void check_rc(int rc);
public:
	sqlite3* db;
	DBclass();
	void insert();
	std::string quotesql(const std::string& s);
	void getSymbols();
	std::vector <struct infoStruct> symbols;
};