#include "ProducerConsumerFibers.h"
#include "TimeUsage.h"
#include "ProducerConsumerBaseTest.h"

//https://stackoverflow.com/questions/6884093/warning-c4003-not-enough-actual-parameters-for-macro-max-visual-studio-2010
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>
#include <boost/bind.hpp>
#include <iostream>

const std::string ProducerConsumerFibers::_className = "Fiber";
const std::string ProducerConsumerFibers::_subClassName = "ProducerConsumer";

void ProducerConsumerFibers::runProducerConsumer(std::vector<std::vector<std::string>>* data)
{
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerFibers::singleOrderedProducerConsumer, _className, _subClassName, "Single", "Ordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerFibers::singleUnorderedProducerConsumer, _className, _subClassName, "Single", "Unordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerFibers::fullOrderedProducerConsumer, _className, _subClassName, "Full", "Ordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerFibers::fullUnorderedProducerConsumer, _className, _subClassName, "Full", "Unordered");
}

void ProducerConsumerFibers::runFullOrderedProducerConsumer(std::vector<std::vector<std::string>>* data) {
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerFibers::fullOrderedProducerConsumer, _className, _subClassName, "Full", "Ordered");
}

void ProducerConsumerFibers::singleOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testSingleOrderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::fibers::fiber> fiberVector(producerCount + consumerCount);

	int largestCounter = producerCount;
	if (producerCount < consumerCount) largestCounter = consumerCount;

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < largestCounter; i++) {
		if (i < producerCount) {
			int toProduceAmount = (remainingProducerInput / remainingProducers);
			fiberVector[i] = boost::fibers::fiber(&ProducerConsumerFibers::singleProducerFunction, toProduceAmount, &numberVector, vectorSize, i);
			remainingProducerInput = remainingProducerInput - toProduceAmount;
			remainingProducers = remainingProducers - 1;
		}
		if (i < consumerCount) {
			int toProduceAmount = (remainingConsumerInput / remainingConsumers);
			fiberVector[producerCount + i] = boost::fibers::fiber(&ProducerConsumerFibers::singleConsumerFunction, toProduceAmount, &numberVector, (producerCount + i));
			remainingConsumerInput = remainingConsumerInput - toProduceAmount;
			remainingConsumers = remainingConsumers - 1;
		}
	}

	for (int i = 0; i < fiberVector.size(); i++) {
		fiberVector[i].join();
	}
}

void ProducerConsumerFibers::singleUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testSingleUnorderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::fibers::fiber> fiberVector(producerCount + consumerCount);

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < producerCount; i++) {
		int toProduceAmount = (remainingProducerInput / remainingProducers);
		fiberVector[i] = boost::fibers::fiber(&ProducerConsumerFibers::singleProducerFunction, toProduceAmount, &numberVector, vectorSize, i);
		remainingProducerInput = remainingProducerInput - toProduceAmount;
		remainingProducers = remainingProducers - 1;
	}

	for (int j = 0; j < consumerCount; j++) {
		int toProduceAmount = (remainingConsumerInput / remainingConsumers);
		fiberVector[producerCount + j] = boost::fibers::fiber(&ProducerConsumerFibers::singleConsumerFunction, toProduceAmount, &numberVector, (producerCount + j));
		remainingConsumerInput = remainingConsumerInput - toProduceAmount;
		remainingConsumers = remainingConsumers - 1;
	}

	for (int i = 0; i < fiberVector.size(); i++) {
		fiberVector[i].join();
	}
}

void ProducerConsumerFibers::singleProducerFunction(int input, std::vector<int>* numberVector, int vectorSize, int producerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (numberVector->size() < vectorSize) {
			numberVector->push_back(counter);
			if (TimeUsage::showDebugPrint(input)) std::cout << "Fiber " << producerNumber << " produced: " << counter << std::endl;
			counter++;
		}
		boost::this_fiber::yield();
	}
}

void ProducerConsumerFibers::singleConsumerFunction(int input, std::vector<int>* numberVector, int consumerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (!numberVector->empty()) {
			int number = numberVector->back();
			numberVector->pop_back();
			counter++;
			if (TimeUsage::showDebugPrint(input)) std::cout << "Fiber " << consumerNumber << " consumed: " << counter << std::endl;
		}
		boost::this_fiber::yield();
	}
}

void ProducerConsumerFibers::fullOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testFullOrderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::fibers::fiber> fiberVector(producerCount + consumerCount);

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
			fiberVector[i] = boost::fibers::fiber(&ProducerConsumerFibers::fullProducerFunction, toProduceAmount, &numberVector, vectorSize, i);
			remainingProducerInput = remainingProducerInput - toProduceAmount;
			remainingProducers = remainingProducers - 1;
		}
		if (i < consumerCount) {
			int toProduceAmount = (remainingConsumerInput / remainingConsumers);
			fiberVector[producerCount + i] = boost::fibers::fiber(&ProducerConsumerFibers::fullConsumerFunction, toProduceAmount, &numberVector, (producerCount + i));
			remainingConsumerInput = remainingConsumerInput - toProduceAmount;
			remainingConsumers = remainingConsumers - 1;
		}
	}


	for (int i = 0; i < fiberVector.size(); i++) {
		fiberVector[i].join();
	}
}

void ProducerConsumerFibers::fullUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testFullUnorderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::fibers::fiber> fiberVector(producerCount + consumerCount);

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < producerCount; i++) {
		int toProduceAmount = (remainingProducerInput / remainingProducers);
		fiberVector[i] = boost::fibers::fiber(&ProducerConsumerFibers::fullProducerFunction, toProduceAmount, &numberVector, vectorSize, i);
		remainingProducerInput = remainingProducerInput - toProduceAmount;
		remainingProducers = remainingProducers - 1;
	}
	for (int j = 0; j < consumerCount; j++) {
		int toProduceAmount = (remainingConsumerInput / remainingConsumers);
		fiberVector[producerCount + j] = boost::fibers::fiber(&ProducerConsumerFibers::fullConsumerFunction, toProduceAmount, &numberVector, (producerCount + j));
		remainingConsumerInput = remainingConsumerInput - toProduceAmount;
		remainingConsumers = remainingConsumers - 1;
	}

	for (int i = 0; i < fiberVector.size(); i++) {
		fiberVector[i].join();
	}
}

void ProducerConsumerFibers::fullProducerFunction(int input, std::vector<int>* numberVector, int vectorSize, int producerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (numberVector->size() < vectorSize) {
			numberVector->push_back(counter);
			if (TimeUsage::showDebugPrint(input)) std::cout << "Fiber " << producerNumber << " produced: " << counter << std::endl;
			counter++;
		}
		else {
			if (TimeUsage::showDebugPrint(input)) std::cout << "Fiber " << producerNumber << " yielded because vector is full. " << std::endl;
			boost::this_fiber::yield();
		}
	}
}

void ProducerConsumerFibers::fullConsumerFunction(int input, std::vector<int>* numberVector, int consumerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (!numberVector->empty()) {
			int number = numberVector->back();
			numberVector->pop_back();
			counter++;
			if (TimeUsage::showDebugPrint(input)) std::cout << "Fiber " << consumerNumber << " consumed: " << number << std::endl;
		}
		else {
			if (TimeUsage::showDebugPrint(input)) std::cout << "Fiber " << consumerNumber << " yielded because vector is empty. " << std::endl;
			boost::this_fiber::yield();
		}
	}
}