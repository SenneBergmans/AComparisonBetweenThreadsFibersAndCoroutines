#pragma once

#include <vector>
#include <string>
#include <boost/thread/thread.hpp>

class ProducerConsumerThreads
{
public:
	/// <summary>
	/// Run the testProducerConsumer test using all parameters from the ProducerConsumer.h file.
	/// </summary>
	/// <param name="data">The vector to which the test results will be appended.</param>
	static void runProducerConsumer(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	/// <summary>
	/// Produce and consume 'input' numbers using 'producerCount' producers and 'consumerCount' consumers.
	/// </summary>
	/// <param name="input">The amount of items to be produced.</param>
	/// <param name="producerCount">The amount of producers that will be used.</param>
	/// <param name="consumerCount">The amount of consumers that will be used.</param>
	/// <param name="vectorSize">The size of the vector that holds the produced numbers.</param>
	static void testProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);

	/// <summary>
	/// Producer that produces 'input' numbers.
	/// </summary>
	/// <param name="input">The amount of items to be produced by this producer.</param>
	/// <param name="numberVector">The vector where the numbers will be stored.</param>
	/// <param name="vectorSize">The max size of 'numberVector'.</param>
	/// <param name="numberVectorMutex">A mutex to ensure only a single thread can access the numberVector at a time.</param>
	/// <param name="producerNumber">The identification number of the thread to help visualisation in debugPrint.</param>
	static void testProducerFunction(int input, std::vector<int>* numberVector, int vectorSize, boost::mutex* numberVectorMutex, int producerNumber);

	/// <summary>
	/// Consumer that consumes  'input' numbers.
	/// </summary>
	/// <param name="input">The amount of items to be consumed by this consumer.</param>
	/// <param name="numberVector">The vector where the numbers will be stored.</param>
	/// <param name="numberVectorMutex">A mutex to ensure only a single thread can access the numberVector at a time.</param>
	/// <param name="consumerNumber">The identification number of the thread to help visualisation in debugPrint.</param>
	static void testConsumerFunction(int input, std::vector<int>* numberVector, boost::mutex* numberVectorMutex, int consumerNumber);
};
