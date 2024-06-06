#pragma once

#include <vector>
#include <string>
#include <boost/coroutine2/all.hpp>

class ProducerConsumerBoostCoroutines
{
public:
	/// <summary>
	/// Run all producer consumer tests using boost coroutines.
	/// 
	/// The tests are singleOrderedProducerConsumer, singleUnorderedProducerConsumer, fullOrderedProducerConsumer, fullUnorderedProducerConsumer
	/// </summary>
	/// <param name="data">The vector to which the test results will be appended.</param>
	static void runProducerConsumer(std::vector<std::vector<std::string>>* data);

	/// <summary>
	/// Run the fullOrderedProducerConsumer test using all parameters from the ProducerConsumer.h file.
	/// </summary>
	/// <param name="data">The vector to which the test results will be appended.</param>
	static void runFullOrderedProducerConsumer(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	/// <summary>
	/// Producer and consumer 'input' numbers using 'producerCount' producers and 'consumerCount' consumers in a single ordered manner.
	/// </summary>
	/// <param name="input">The amount of items to be produced.</param>
	/// <param name="producerCount">The amount of producers that will be used.</param>
	/// <param name="consumerCount">The amount of consumers that will be used.</param>
	/// <param name="vectorSize">The size of the vector that holds the produced numbers.</param>
	static void singleOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);

	/// <summary>
	/// Producer and consumer 'input' numbers using 'producerCount' producers and 'consumerCount' consumers in a single unordered manner.
	/// </summary>
	/// <param name="input">The amount of items to be produced.</param>
	/// <param name="producerCount">The amount of producers that will be used.</param>
	/// <param name="consumerCount">The amount of consumers that will be used.</param>
	/// <param name="vectorSize">The size of the vector that holds the produced numbers.</param>
	static void singleUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);
	
	/// <summary>
	/// Producer that produces a single item and yields.
	/// </summary>
	/// <param name="active">The push type coroutine used to yield.</param>
	/// <param name="input">The amount of items to be produced by this producer.</param>
	/// <param name="numberVector">The vector where the numbers will be stored.</param>
	/// <param name="vectorSize">The max size of 'numberVector'.</param>
	/// <param name="producerNumber">The identification number of the coroutine to help visualisation in debugPrint.</param>
	static void singleProducerFunction(boost::coroutines2::coroutine<void>::push_type& active, int input, std::vector<int>* numberVector, int vectorSize, int producerNumber);
	
	/// <summary>
	/// Consumer that consumes a single item and yields.
	/// </summary>
	/// <param name="active">The push type coroutine used to yield.</param>
	/// <param name="input">The amount of items to be consumed by this consumer.</param>
	/// <param name="numberVector">The vector where the numbers will be stored.</param>
	/// <param name="consumerNumber">The identification number of the coroutine to help visualisation in debugPrint.</param>
	static void singleConsumerFunction(boost::coroutines2::coroutine<void>::push_type& active, int input, std::vector<int>* numberVector, int consumerNumber);

	/// <summary>
	/// Producer and consumer 'input' numbers using 'producerCount' producers and 'consumerCount' consumers in a full ordered manner.
	/// </summary>
	/// <param name="input">The amount of items to be produced.</param>
	/// <param name="producerCount">The amount of producers that will be used.</param>
	/// <param name="consumerCount">The amount of consumers that will be used.</param>
	/// <param name="vectorSize">The size of the vector that holds the produced numbers.</param>
	static void fullOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);

	/// <summary>
	/// Producer and consumer 'input' numbers using 'producerCount' producers and 'consumerCount' consumers in a full unordered manner.
	/// </summary>
	/// <param name="input">The amount of items to be produced.</param>
	/// <param name="producerCount">The amount of producers that will be used.</param>
	/// <param name="consumerCount">The amount of consumers that will be used.</param>
	/// <param name="vectorSize">The size of the vector that holds the produced numbers.</param>
	static void fullUnorderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize);

	/// <summary>
	/// Producer that produces until the vector is full and yields.
	/// </summary>
	/// <param name="active">The push type coroutine used to yield.</param>
	/// <param name="input">The amount of items to be produced by this producer.</param>
	/// <param name="numberVector">The vector where the numbers will be stored.</param>
	/// <param name="vectorSize">The max size of 'numberVector'.</param>
	/// <param name="producerNumber">The identification number of the coroutine to help visualisation in debugPrint.</param>
	static void fullProducerFunction(boost::coroutines2::coroutine<void>::push_type& active, int input, std::vector<int>* numberVector, int vectorSize, int producerNumber);

	/// <summary>
	/// Consumer that consumes until the vector is empty and yields.
	/// </summary>
	/// <param name="active">The push type coroutine used to yield.</param>
	/// <param name="input">The amount of items to be consumed by this consumer.</param>
	/// <param name="numberVector">The vector where the numbers will be stored.</param>
	/// <param name="consumerNumber">The identification number of the coroutine to help visualisation in debugPrint.</param>
	static void fullConsumerFunction(boost::coroutines2::coroutine<void>::push_type& active, int input, std::vector<int>* numberVector, int consumerNumber);
};

