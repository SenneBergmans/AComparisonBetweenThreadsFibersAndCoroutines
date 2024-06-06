#include "ProducerConsumerTest.h"
#include "ProducerConsumerBoostCoroutine.h"
#include "ProducerConsumerFibers.h"
#include "ProducerConsumerCoroutinesTS.h"
#include "ProducerConsumerThreads.h"

std::vector<std::vector<std::string>> ProducerConsumerTest::testFunctions(std::vector<std::string> testInformation) {
	testInformation.push_back("FillType");
	testInformation.push_back("Order");
	testInformation.push_back("Sample");
	testInformation.push_back("VectorSize");
	testInformation.push_back("ProducerCount");
	testInformation.push_back("ConsumerCount");
	testInformation.push_back("ProducedAmount");
	testInformation.push_back("Time");

	std::vector<std::vector<std::string>> data = { testInformation };

	std::cout << "Boost coroutine" << std::endl;
	ProducerConsumerBoostCoroutines::runFullOrderedProducerConsumer(&data);
	std::cout << "Fiber" << std::endl;
	ProducerConsumerFibers::runFullOrderedProducerConsumer(&data);
	std::cout << "Coroutine TS" << std::endl;
	ProducerConsumerCoroutinesTS::runFullOrderedProducerConsumer(&data);
	std::cout << "Thread" << std::endl;
	ProducerConsumerThreads::runProducerConsumer(&data);

	return data;
}