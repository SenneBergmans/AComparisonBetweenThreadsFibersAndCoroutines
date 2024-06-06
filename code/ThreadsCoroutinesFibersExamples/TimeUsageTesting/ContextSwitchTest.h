#pragma once

#include "TimeUsage.h"

class ContextSwitchTest : public TimeUsage
{
public:
	ContextSwitchTest() : TimeUsage() {}

protected:
	std::vector<std::vector<std::string>> testFunctions(std::vector<std::string> testInformation);
};

