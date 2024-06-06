#pragma once

#include "CoroutineHandler.h"

#include <vector>
#include <string>

class ProducerConsumerCoroutinesTS
{
public:
	static void runProducerConsumer(std::vector<std::vector<std::string>>* data);
	static void runFullOrderedProducerConsumer(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	static void singleOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);
	static void singleUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);
	static CoroutineHandler<bool> singleProducerFunction(int input, std::vector<int>* numberVector, int vectorSize, int producerNumber);
	static CoroutineHandler<bool> singleConsumerFunction(int input, std::vector<int>* numberVector, int consumerNumber);

	static void fullOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);
	static void fullUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);
	static CoroutineHandler<bool> fullProducerFunction(int input, std::vector<int>* numberVector, int vectorSize, int producerNumber);
	static CoroutineHandler<bool> fullConsumerFunction(int input, std::vector<int>* numberVector, int consumerNumber);
};

