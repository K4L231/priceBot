#include <iostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include "symbolSource.h"

class DBclass {
private:
	char* err;
	int rc;
	int check_rc(int rc);
	void insertToSymbols(std::string symbol, std::string source);
public:
	sqlite3* db;
	DBclass();
	void insert();
	std::string quotesql(const std::string& s);
	void getSymbols();
	std::vector <struct infoStruct> symbols;
};