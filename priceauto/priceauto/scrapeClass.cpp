#include "scrapeClass.h"


void scrapeClass::init(std::string symbol, std::string source)
{
	this->symbol = symbol;
	this->source = source;
}

void scrapeClass::print() {
	std::cout << this->symbol << std::endl;
}
