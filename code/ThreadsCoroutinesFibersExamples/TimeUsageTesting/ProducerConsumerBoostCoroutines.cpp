#include "ProducerConsumerBoostCoroutine.h"
#include "TimeUsage.h"
#include "ProducerConsumerParameters.h"
#include "BoostCoroutineHelper.h"
#include "ProducerConsumerBaseTest.h"

#include <boost/bind.hpp>
#include <iostream>

const std::string ProducerConsumerBoostCoroutines::_className = "BoostCoroutine";
const std::string ProducerConsumerBoostCoroutines::_subClassName = "ProducerConsumer";

void ProducerConsumerBoostCoroutines::runProducerConsumer(std::vector<std::vector<std::string>>* data) {
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerBoostCoroutines::singleOrderedProducerConsumer, _className, _subClassName, "Single", "Ordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerBoostCoroutines::singleUnorderedProducerConsumer, _className, _subClassName, "Single", "Unordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerBoostCoroutines::fullOrderedProducerConsumer, _className, _subClassName, "Full", "Ordered");
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerBoostCoroutines::fullUnorderedProducerConsumer, _className, _subClassName, "Full", "Unordered");
}

void ProducerConsumerBoostCoroutines::runFullOrderedProducerConsumer(std::vector<std::vector<std::string>>* data) {
	ProducerConsumerBaseTest::runProducerConsumerTest(data, &ProducerConsumerBoostCoroutines::fullOrderedProducerConsumer, _className, _subClassName, "Full", "Ordered");
}

void ProducerConsumerBoostCoroutines::singleOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testSingleOrderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::coroutines2::coroutine<void>::pull_type> coroutineVector{};

	int largestCounter = producerCount;
	if (producerCount < consumerCount) largestCounter = consumerCount;

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < largestCounter; i++) {
		if (i < producerCount) {
			int toProduceAmount = (remainingProducerInput / remainingProducers);
			boost::coroutines2::coroutine<void>::pull_type producerCoroutine(boost::bind(ProducerConsumerBoostCoroutines::singleProducerFunction, boost::placeholders::_1, toProduceAmount, &numberVector, vectorSize, i));
			coroutineVector.push_back(std::move(producerCoroutine));
			remainingProducerInput = remainingProducerInput - toProduceAmount;
			remainingProducers = remainingProducers - 1;
		}
		if (i < consumerCount) {
			int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
			boost::coroutines2::coroutine<void>::pull_type consumerCoroutine(boost::bind(ProducerConsumerBoostCoroutines::singleConsumerFunction, boost::placeholders::_1, toConsumeAmount, &numberVector, (producerCount + i)));
			coroutineVector.push_back(std::move(consumerCoroutine));
			remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
			remainingConsumers = remainingConsumers - 1;
		}
	}

	BoostCoroutineHelper::joinCoroutines(&coroutineVector);
}

void ProducerConsumerBoostCoroutines::singleUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testSingleUnorderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::coroutines2::coroutine<void>::pull_type> coroutineVector{};

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < producerCount; i++) {
		int toProduceAmount = (remainingProducerInput / remainingProducers);
		boost::coroutines2::coroutine<void>::pull_type producerCoroutine(boost::bind(ProducerConsumerBoostCoroutines::singleProducerFunction, boost::placeholders::_1, toProduceAmount, &numberVector, vectorSize, i));
		coroutineVector.push_back(std::move(producerCoroutine));
		remainingProducerInput = remainingProducerInput - toProduceAmount;
		remainingProducers = remainingProducers - 1;
	}
	for (int j = 0; j < consumerCount; j++) {
		int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
		boost::coroutines2::coroutine<void>::pull_type consumerCoroutine(boost::bind(ProducerConsumerBoostCoroutines::singleConsumerFunction, boost::placeholders::_1, toConsumeAmount, &numberVector, (producerCount + j)));
		coroutineVector.push_back(std::move(consumerCoroutine));
		remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
		remainingConsumers = remainingConsumers - 1;
	}


	BoostCoroutineHelper::joinCoroutines(&coroutineVector);
}

void ProducerConsumerBoostCoroutines::singleProducerFunction(boost::coroutines2::coroutine<void>::push_type& active, int input, std::vector<int>* numberVector, int vectorSize, int producerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (numberVector->size() < vectorSize) {
			numberVector->push_back(counter);
			if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << producerNumber << " produced: " << counter << std::endl;
			counter++;
		}
		active();
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << producerNumber << " ended." << std::endl;
}

void ProducerConsumerBoostCoroutines::singleConsumerFunction(boost::coroutines2::coroutine<void>::push_type& active, int input, std::vector<int>* numberVector, int consumerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (!numberVector->empty()) {
			int number = numberVector->back();
			numberVector->pop_back();
			counter++;
			if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << consumerNumber << " consumed: " << number << std::endl;
		}
		active();
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << consumerNumber << " ended." << std::endl;
}

void ProducerConsumerBoostCoroutines::fullOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testFullOrderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::coroutines2::coroutine<void>::pull_type> coroutineVector{};


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
			boost::coroutines2::coroutine<void>::pull_type producerCoroutine(boost::bind(ProducerConsumerBoostCoroutines::fullProducerFunction, boost::placeholders::_1, toProduceAmount, &numberVector, vectorSize, i));
			coroutineVector.push_back(std::move(producerCoroutine));
			remainingProducerInput = remainingProducerInput - toProduceAmount;
			remainingProducers = remainingProducers - 1;
		}
		if (i < consumerCount) {
			int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
			boost::coroutines2::coroutine<void>::pull_type consumerCoroutine(boost::bind(ProducerConsumerBoostCoroutines::fullConsumerFunction, boost::placeholders::_1, toConsumeAmount, &numberVector, (producerCount + i)));
			coroutineVector.push_back(std::move(consumerCoroutine));
			remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
			remainingConsumers = remainingConsumers - 1;
		}
	}

	BoostCoroutineHelper::joinCoroutines(&coroutineVector);
}

void ProducerConsumerBoostCoroutines::fullUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testFullUnorderedProducerConsumer " << std::to_string(input) << " " << std::to_string(producerCount) << " " << std::to_string(consumerCount) << " " << std::to_string(vectorSize) << std::endl;

	std::vector<int> numberVector = std::vector<int>();
	std::vector<boost::coroutines2::coroutine<void>::pull_type> coroutineVector{};

	int remainingProducerInput = input;
	int remainingProducers = producerCount;
	int remainingConsumerInput = input;
	int remainingConsumers = consumerCount;

	for (int i = 0; i < producerCount; i++) {
		int toProduceAmount = (remainingProducerInput / remainingProducers);
		boost::coroutines2::coroutine<void>::pull_type producerCoroutine(boost::bind(ProducerConsumerBoostCoroutines::fullProducerFunction, boost::placeholders::_1, toProduceAmount, &numberVector, vectorSize, i));
		coroutineVector.push_back(std::move(producerCoroutine));
		remainingProducerInput = remainingProducerInput - toProduceAmount;
		remainingProducers = remainingProducers - 1;
	}
	for (int j = 0; j < consumerCount; j++) {
		int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
		boost::coroutines2::coroutine<void>::pull_type consumerCoroutine(boost::bind(ProducerConsumerBoostCoroutines::fullConsumerFunction, boost::placeholders::_1, toConsumeAmount, &numberVector, (producerCount + j)));
		coroutineVector.push_back(std::move(consumerCoroutine));
		remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
		remainingConsumers = remainingConsumers - 1;
	}


	BoostCoroutineHelper::joinCoroutines(&coroutineVector);
}

void ProducerConsumerBoostCoroutines::fullProducerFunction(boost::coroutines2::coroutine<void>::push_type& active, int input, std::vector<int>* numberVector, int vectorSize, int producerNumber)
{
	int counter = 0;
	while (counter < input) {
		if (numberVector->size() < vectorSize) {
			numberVector->push_back(counter);
			if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << producerNumber << " produced: " << counter << std::endl;
			counter++;
		}
		else {
			active();
		}
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << producerNumber << " ended." << std::endl;
}

void ProducerConsumerBoostCoroutines::fullConsumerFunction(boost::coroutines2::coroutine<void>::push_type& active, int input, std::vector<int>* numberVector, int consumerNumber)
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
			active();
		}
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << consumerNumber << " ended." << std::endl;
}