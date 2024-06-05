#pragma once

#include <boost/coroutine2/all.hpp>
#include <vector>

class StackfulCoroutinesProducerConsumer
{
public:
	static void runSingle();
	static void runOrderedMulti();
private:
	static void producerFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::vector<int>* numberVector, int producedConsumedAmount, int index);
	static void consumerFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::vector<int>* numberVector, int producedConsumedAmount, int index);
};

