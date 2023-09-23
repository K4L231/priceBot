#ifndef userToken_H
#define userToken_H
#include <string>

struct userToken {
	std::string name;
	std::string token;
	double limit;
	bool exists;
};

#endif // !SYMBOLSOURCE_H
