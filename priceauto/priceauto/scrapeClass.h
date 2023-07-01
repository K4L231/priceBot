#include <iostream>
#include <string>
class scrapeClass {
private:
public:
	void init(std::string symbol, std::string source);
	void print();
	std::string symbol;
	std::string source;
};