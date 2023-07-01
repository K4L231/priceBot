#include <iostream>
#include <string>
class scrapeClass {
private:
	void scrapeBinance();
	std::string symbol;
	std::string source;
public:
	void init(std::string symbol, std::string source);
	void print();
	void scrapeMain();
};