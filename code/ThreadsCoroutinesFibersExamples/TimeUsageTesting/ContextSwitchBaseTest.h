#pragma once

#include "GlobalTestParameters.h"
#include "ContextSwitchParameters.h"
#include "TimeUsage.h"

#include <boost/bind.hpp>
#include <vector>
#include <string>
#include <iostream>

class ContextSwitchBaseTest
{
public:
	template <typename F>
	static void runContextSwitchTest(std::vector<std::vector<std::string>>* data, F&& functionPointer, std::string className, std::string functionName) {
		for (int sample = 1; sample <= GlobalTestParameters::sampleSize; sample++) {
			std::cout << className << " " << _testName << " " << functionName << " " << std::to_string(sample) << std::endl;
			for (int contextSwitchCount = ContextSwitchParameters::contextSwitchMin; contextSwitchCount <= ContextSwitchParameters::contextSwitchMax; contextSwitchCount += ContextSwitchParameters::contextSwitchStepSize) {
				std::string time = TimeUsage::testSoloFunction(boost::bind(functionPointer, contextSwitchCount));
				std::vector<std::string> testData{ className, _testName, functionName, std::to_string(sample), std::to_string(contextSwitchCount), time };
				data->push_back(testData);
			}
		}
	}

private:
	const static std::string _testName;
};

