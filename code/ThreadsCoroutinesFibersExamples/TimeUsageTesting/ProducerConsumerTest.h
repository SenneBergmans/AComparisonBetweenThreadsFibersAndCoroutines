#pragma once

#include "TimeUsage.h"

class ProducerConsumerTest : public TimeUsage
{
public:
	ProducerConsumerTest() : TimeUsage() {}

protected:
	std::vector<std::vector<std::string>> testFunctions(std::vector<std::string> testInformation);
};

