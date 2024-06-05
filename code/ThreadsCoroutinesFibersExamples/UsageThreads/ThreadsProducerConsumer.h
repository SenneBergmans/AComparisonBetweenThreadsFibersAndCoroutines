#pragma once

#include <vector>
#include <boost/thread.hpp>

class ThreadsProducerConsumer
{
public:
	static void runExample();
private:
	static void producerFunction(std::vector<int>* numberVector, boost::mutex* threadMutex);
	static void consumerFunction(std::vector<int>* numberVector, boost::mutex* threadMutex);
};