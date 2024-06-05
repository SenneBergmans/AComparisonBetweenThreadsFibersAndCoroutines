#pragma once

#include <vector>
#include <boost/coroutine2/all.hpp>

class FibersProducerConsumer
{
public: 
	static void runSingle();
	static void runUnorderedMulti();
	static void runOrderedMulti();
private:
	static void producerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index);
	static void consumerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index);
};

