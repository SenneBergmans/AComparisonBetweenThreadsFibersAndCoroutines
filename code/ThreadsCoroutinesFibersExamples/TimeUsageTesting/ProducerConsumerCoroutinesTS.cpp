#include "ProducerConsumerCoroutinesTS.h"
#include "TimeUsage.h"
#include "ProducerConsumerParameters.h"
#include "CoroutineTSHelper.h"
#include "ProducerConsumerBaseTest.h"

#include <boost/bind.hpp>
#include <iostream>

const std::string ProducerConsumerCoroutinesTS::_className = "CoroutineTS";
const std::string ProducerConsumerCoroutinesTS::_subClassName = "ProducerConsumer";

void ProducerConsumerCoroutinesTS::runProducerConsumer(std::vector<std::vector<std::string>>* data)
{
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerCoroutinesTS::singleOrderedProducerConsumer, _className, _subClassName, "Single", "Ordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerCoroutinesTS::singleUnorderedProducerConsumer, _className, _subClassName, "Single", "Unordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerCoroutinesTS::fullOrderedProducerConsumer, _className, _subClassName, "Full", "Ordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerCoroutinesTS::fullUnorderedProducerConsumer, _className, _subClassName, "Full", "Unordered");
}

void ProducerConsumerCoroutinesTS::runFullOrderedProducerConsumer(std::vector<std::vector<std::string>>* data)
{
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerCoroutinesTS::fullOrderedProducerConsumer, _className, _subClassName, "Full", "Ordered");
}

void ProducerConsumerCoroutinesTS::singleOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testSingleOrderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<CoroutineHandler<bool>> coroutineVector = {};

	int largestCounter = producerCount;
	if (producerCount < consumerCount) largestCounter = consumerCount;

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < largestCounter; i++) {
		if (i < producerCount) {
			int toProduceAmount = (remainingProducerInput / remainingProducers);
			coroutineVector.push_back(singleProducerFunction(toProduceAmount, &numberVector, vectorSize, i));
			remainingProducerInput = remainingProducerInput - toProduceAmount;
			remainingProducers = remainingProducers - 1;
		}
		if (i < consumerCount) {
			int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
			coroutineVector.push_back(singleConsumerFunction(toConsumeAmount, &numberVector, (producerCount + i)));
			remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
			remainingConsumers = remainingConsumers - 1;
		}
	}

	CoroutineTSHelper::joinCoroutines(&coroutineVector);
}

void ProducerConsumerCoroutinesTS::singleUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testSingleUnorderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<CoroutineHandler<bool>> coroutineVector = {};

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < producerCount; i++) {
		int toProduceAmount = (remainingProducerInput / remainingProducers);
		coroutineVector.push_back(singleProducerFunction(toProduceAmount, &numberVector, vectorSize, i));
		remainingProducerInput = remainingProducerInput - toProduceAmount;
		remainingProducers = remainingProducers - 1;
	}
	for (int j = 0; j < consumerCount; j++) {
		int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
		coroutineVector.push_back(singleConsumerFunction(toConsumeAmount, &numberVector, (producerCount + j)));
		remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
		remainingConsumers = remainingConsumers - 1;
	}

	CoroutineTSHelper::joinCoroutines(&coroutineVector);
}

CoroutineHandler<bool> ProducerConsumerCoroutinesTS::singleProducerFunction(int input, std::vector<int>* numberVector, int vectorSize, int producerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (numberVector->size() < vectorSize) {
			numberVector->push_back(counter);
			if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << producerNumber << " produced: " << counter << std::endl;
			counter++;
		}
		co_yield true;
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << producerNumber << " ended." << std::endl;
	co_return;
}

CoroutineHandler<bool> ProducerConsumerCoroutinesTS::singleConsumerFunction(int input, std::vector<int>* numberVector, int consumerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (!numberVector->empty()) {
			int number = numberVector->back();
			numberVector->pop_back();
			counter++;
			if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << consumerNumber << " consumed: " << number << std::endl;
		}
		co_yield true;
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << consumerNumber << " ended." << std::endl;
	co_return;
}

void ProducerConsumerCoroutinesTS::fullOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<CoroutineHandler<bool>> coroutineVector = {};

	int largestCounter = producerCount;
	if (producerCount < consumerCount) largestCounter = consumerCount;

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < largestCounter; i++) {
		if (i < producerCount) {
			int toProduceAmount = (remainingProducerInput / remainingProducers);
			coroutineVector.push_back(fullProducerFunction(toProduceAmount, &numberVector, vectorSize, i));
			remainingProducerInput = remainingProducerInput - toProduceAmount;
			remainingProducers = remainingProducers - 1;
		}
		if (i < consumerCount) {
			int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
			coroutineVector.push_back(fullConsumerFunction(toConsumeAmount, &numberVector, (producerCount + i)));
			remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
			remainingConsumers = remainingConsumers - 1;
		}
	}

	CoroutineTSHelper::joinCoroutines(&coroutineVector);
}

void ProducerConsumerCoroutinesTS::fullUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testFullUnorderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<CoroutineHandler<bool>> coroutineVector = {};

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < producerCount; i++) {
		int toProduceAmount = (remainingProducerInput / remainingProducers);
		coroutineVector.push_back(fullProducerFunction(toProduceAmount, &numberVector, vectorSize, i));
		remainingProducerInput = remainingProducerInput - toProduceAmount;
		remainingProducers = remainingProducers - 1;
	}
	for (int j = 0; j < consumerCount; j++) {
		int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
		coroutineVector.push_back(fullConsumerFunction(toConsumeAmount, &numberVector, (producerCount + j)));
		remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
		remainingConsumers = remainingConsumers - 1;
	}

	CoroutineTSHelper::joinCoroutines(&coroutineVector);
}

CoroutineHandler<bool> ProducerConsumerCoroutinesTS::fullProducerFunction(int input, std::vector<int>* numberVector, int vectorSize, int producerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (numberVector->size() < vectorSize) {
			numberVector->push_back(counter);
			if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << producerNumber << " produced: " << counter << std::endl;
			counter++;
		}
		else {
			co_yield true;
		}
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << producerNumber << " ended." << std::endl;
	co_return;
}

CoroutineHandler<bool> ProducerConsumerCoroutinesTS::fullConsumerFunction(int input, std::vector<int>* numberVector, int consumerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (!numberVector->empty()) {
			int number = numberVector->back();
			numberVector->pop_back();
			counter++;
			if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << consumerNumber << " consumed: " << number << std::endl;
		}
		else {
			co_yield true;
		}
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << consumerNumber << " ended." << std::endl;
	co_return;
}