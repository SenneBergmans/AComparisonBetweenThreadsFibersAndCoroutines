#pragma once
#include "GlobalTestParameters.h"
#include "MergesortParameters.h"
#include "TimeUsage.h"

#include <boost/bind.hpp>
#include <vector>
#include <string>
#include <iostream>

class MergesortBaseTest
{
public:
	template <typename F>
	static void runSingleThreadTest(std::vector<std::vector<std::string>>* data, F&& functionPointer, std::string className) {
		const std::string functionName = "SingleThreadMergesort";
		const std::string threadCount = "1";

		for (int sample = 1; sample <= GlobalTestParameters::sampleSize; sample++) {
			std::cout << className << " " << _testName << " " << functionName << " " << std::to_string(sample) << std::endl;
			for (int totalAmount = MergesortParameters::totalAmountMin; totalAmount <= MergesortParameters::totalAmountMax; totalAmount += MergesortParameters::totalAmountStepSize) {
				for (int splitThreshold = MergesortParameters::splitThresholdMin; splitThreshold <= MergesortParameters::splitThresholdMax; splitThreshold += MergesortParameters::splitThresholdStepSize) {
					std::string time = TimeUsage::testSoloFunction(boost::bind(functionPointer, totalAmount, splitThreshold));
					std::vector<std::string> testData{ className, _testName, functionName, threadCount, std::to_string(sample), std::to_string(totalAmount), std::to_string(splitThreshold), time };
					data->push_back(testData);
				}
			}
		}
	}

	template <typename F>
	static void runMultiThreadTest(std::vector<std::vector<std::string>>* data, F&& functionPointer, std::string className) {
		const std::string functionName = "MultiThreadMergesort";

		for (int sample = 1; sample <= GlobalTestParameters::sampleSize; sample++) {
			std::cout << className << " " << _testName << " " << functionName << " " << std::to_string(sample) << std::endl;
			for (int totalAmount = MergesortParameters::totalAmountMin; totalAmount <= MergesortParameters::totalAmountMax; totalAmount += MergesortParameters::totalAmountStepSize) {
				for (int splitThreshold = MergesortParameters::splitThresholdMin; splitThreshold <= MergesortParameters::splitThresholdMax; splitThreshold += MergesortParameters::splitThresholdStepSize) {
					for (int threadCount = MergesortParameters::threadCountMin; threadCount <= MergesortParameters::threadCountMax; threadCount += MergesortParameters::threadCountStepSize) {
						std::string time = TimeUsage::testSoloFunction(boost::bind(functionPointer, totalAmount, splitThreshold, threadCount));
						std::vector<std::string> testData{ className, _testName, functionName, std::to_string(threadCount), std::to_string(sample), std::to_string(totalAmount), std::to_string(splitThreshold), time };
						data->push_back(testData);
					}
				}
			}
		}
	}

private:
	const static std::string _testName;
};

