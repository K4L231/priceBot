#include <string>

class timeClass {
private:


public:
	uint64_t currentTime;
	uint64_t min3epoch;
	uint64_t min5epoch;
	uint64_t min15epoch;
	uint64_t min30epoch;
	uint64_t h1epoch;
	uint64_t h2epoch;
	uint64_t h4epoch;
	uint64_t h6epoch;
	uint64_t h8epoch;
	uint64_t h12epoch;
	uint64_t timeSinceEpochMillisec();
	void getCurrentTime();
	timeClass();

};