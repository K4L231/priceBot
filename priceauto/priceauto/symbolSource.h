#ifndef SYMBOLSOURCE_H
#define SYMBOLSOURCE_H
#include <string>

struct infoStruct {
	std::string symbol;
	std::string source;
	double change;
	double prcntChange;
	double high;
	double low;
	double last;
	double open;
	double volume;
	std::string quoteVolume;
	std::string openTime;
	int responseCode;
};

#endif // !SYMBOLSOURCE_H
