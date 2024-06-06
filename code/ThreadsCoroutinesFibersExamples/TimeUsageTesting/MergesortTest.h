#pragma once

#include "TimeUsage.h"

class MergesortTest : public TimeUsage {
public:
	MergesortTest() : TimeUsage() {}

protected:
	std::vector<std::vector<std::string>> testFunctions(std::vector<std::string> testInformation);
};

