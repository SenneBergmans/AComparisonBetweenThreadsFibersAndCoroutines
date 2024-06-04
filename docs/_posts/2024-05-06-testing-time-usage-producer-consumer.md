---
layout: post
title:  "Testing - TimeUsage - Producer-consumer"
date:   2024-05-06 12:00:00 +0200
categories: testing TimeUsage
permalink: "/testing/TimeUsage/producer-consumer"
---

The producer-consumer example was chosen to get an idea of the performance of the technologies in a more complex test. The example was chosen because it is the same example used to compare the code usage of the technologies.

The variable parameters of the test are the producer count, consumer count, vector size, and produced amount.
The producer and consumer counts can be set individually, but having more producers than consumers or vice versa does not improve the test results.
In general, they are kept at the same amount.

For threads and coroutines, multiple versions of the test have been made.
These versions differ in the order in which the fibers or coroutines are called and their yield behavior.
The version that alternates between having a producer and a consumer active and only yields them when the vector is full or empty performed the best.
This version is the one that is used when comparing different technologies to give each technology the best possible chances.

The test starts by first creating the required amount of producers and consumers.
After that, they all get joined or called until they have finished.
The following example shows this function for fibers.
The middle part of the code ensures that the fibers are created by alternating between producers and consumers until the required numbers of producers and consumers have been made.
The total amount to produce or consume also gets distributed to all producers and consumers.

{% highlight C++ %}
void ProducerConsumerFibers::fullOrderedProducerConsumer(int input, int producerCount, int consumerCount, int vectorSize)
{
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
{% endhighlight %}

The producer and consumer functions are the same as those used in the producer-consumer example.

{% highlight C++ %}
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
{% endhighlight %}