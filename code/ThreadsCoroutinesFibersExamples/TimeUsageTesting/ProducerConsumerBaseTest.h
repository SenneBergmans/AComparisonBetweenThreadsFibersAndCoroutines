#pragma once

#include "GlobalTestParameters.h"
#include "ProducerConsumerParameters.h"
#include "TimeUsage.h"

#include <boost/bind.hpp>
#include <vector>
#include <string>
#include <iostream>

class ProducerConsumerBaseTest
{
public:
	template <typename F>
	static void runProducerConsumerTest(std::vector<std::vector<std::string>>* data, F&& functionPointer, std::string className, std::string subClassName, std::string fillType, std::string order) {
		const std::string functionName = "ProducerConsumer";
		for (int sample = 1; sample <= GlobalTestParameters::sampleSize; sample++) {
			std::cout << className << " " << subClassName << " " << functionName << " " << fillType << " " << order << " " << std::to_string(sample) << std::endl;
			for (int producedAmount = ProducerConsumerParameters::producedAmountMin; producedAmount <= ProducerConsumerParameters::producedAmountMax; producedAmount += ProducerConsumerParameters::producedAmountStepSize) {
				for (int producerCount = ProducerConsumerParameters::producerCountMin; producerCount <= ProducerConsumerParameters::producerCountMax; producerCount += ProducerConsumerParameters::producerCountStepSize) {
					for (int consumerCount = ProducerConsumerParameters::consumerCountMin; consumerCount <= ProducerConsumerParameters::consumerCountMax; consumerCount += ProducerConsumerParameters::consumerCountStepSize) {
						for (int vectorSize = ProducerConsumerParameters::vectorSizeMin; vectorSize <= ProducerConsumerParameters::vectorSizeMax; vectorSize += ProducerConsumerParameters::vectorSizeStepSize) {
							std::string time = TimeUsage::testSoloFunction(boost::bind(functionPointer, producedAmount, producerCount, producerCount, vectorSize));
							std::vector<std::string> testData{ className, subClassName, functionName, fillType, order, std::to_string(sample), std::to_string(vectorSize), std::to_string(producerCount), std::to_string(consumerCount), std::to_string(producedAmount), time};
							data->push_back(testData);
						}
					}
				}
			}
		}
	}
};

