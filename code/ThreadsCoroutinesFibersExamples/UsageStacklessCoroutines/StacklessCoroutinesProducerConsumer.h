#pragma once

#include <vector>
#include <coroutine>
#include "CoroutineHandler.h"

class StacklessCoroutinesProducerConsumer
{
public:
	static void runSingle();
	static void runOrderedMulti();
private:
	static CoroutineHandler<bool> producerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index);
	static CoroutineHandler<bool> consumerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index);
};

