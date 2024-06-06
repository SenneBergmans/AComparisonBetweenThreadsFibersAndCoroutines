#include "ProducerConsumerThreads.h"
#include "TimeUsage.h"
#include "ProducerConsumerBaseTest.h"

#include <boost/bind.hpp>
#include <iostream>

const std::string ProducerConsumerThreads::_className = "Thread";
const std::string ProducerConsumerThreads::_subClassName = "ProducerConsumer";

void ProducerConsumerThreads::runProducerConsumer(std::vector<std::vector<std::string>>* data)
{
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerThreads::testProducerConsumer, _className, _subClassName, "Thread", "Thread");
}

void ProducerConsumerThreads::testProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testProducerConsumer " << std::to_string(input) << std::endl;

	boost::mutex numberVectorMutex;
	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::thread> threadVector(producerCount + consumerCount);

	int largestCounter = producerCount;
	if (producerCount < consumerCount) {
		largestCounter = consumerCount;
	}

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < largestCounter; i++) {
		if (i < producerCount) {
			int toProduceAmount = (remainingProducerInput / remainingProducers);
			threadVector[i] = boost::thread(&ProducerConsumerThreads::testProducerFunction, toProduceAmount, &numberVector, vectorSize, &numberVectorMutex, i);
			remainingProducerInput = remainingProducerInput - toProduceAmount;
			remainingProducers = remainingProducers - 1;
		}
		if (i < consumerCount) {
			int toConsume = (remainingConsumerInput / remainingConsumers);
			threadVector[producerCount + i] = boost::thread(&ProducerConsumerThreads::testConsumerFunction, toConsume, &numberVector, &numberVectorMutex, (producerCount + i));
			remainingConsumerInput = remainingConsumerInput - toConsume;
			remainingConsumers = remainingConsumers - 1;
		}
	}


	for (int i = 0; i < threadVector.size(); i++) {
		threadVector[i].join();
	}
}

void ProducerConsumerThreads::testProducerFunction(int input, std::vector<int>* numberVector, int vectorSize, boost::mutex* numberVectorMutex, int producerNumber)
{
	int counter = 0;
	while (counter < input) {
		boost::lock_guard<boost::mutex> guard(*numberVectorMutex);
		if (numberVector->size() < vectorSize) {
			numberVector->push_back(counter);
			if (TimeUsage::showDebugPrint(input)) std::cout << "Thread " << std::to_string(producerNumber) << " produced: " << counter << std::endl;
			counter++;
		}
	}
}

void ProducerConsumerThreads::testConsumerFunction(int input, std::vector<int>* numberVector, boost::mutex* numberVectorMutex, int consumerNumber)
{
	int counter = 0;
	while (counter < input) {
		boost::lock_guard<boost::mutex> guard(*numberVectorMutex);
		if (!numberVector->empty()) {
			int number = numberVector->back();
			numberVector->pop_back();
			counter++;
			if (TimeUsage::showDebugPrint(input)) std::cout << "Thread " << std::to_string(consumerNumber) << " consumed: " << number << std::endl;
		}
	}
}
