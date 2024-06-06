---
layout: post
title:  "Testing - TimeUsage - Mergesort"
date:   2024-05-06 13:00:00 +0200
categories: testing TimeUsage
permalink: "/testing/TimeUsage/Mergesort"
description: "Mergesort"
---

The Mergesort test was added to measure the performance of the technologies in a way that can benefit from parallelism.
Because only a single vector is used in the producer-consumer test, threads could not fully use their ability to work in parallel.
The Mergesort test is set up so that working in parallel should be possible.

Before each test starts, a file containing a predefined number of numbers is created.
Each test will copy the numbers from this file and sort them into a vector.
The test will time the total duration to generate this sorted vector.
The default version of the test using fibers is the following: 
{% highlight C++ %}
void MergesortFibers::singleThreadMergesort(int totalAmount, int splitThreshold) {
	std::vector<int> singleThreadSortedVector{};
	mergesortFiberFunction(&singleThreadSortedVector, splitThreshold, totalAmount, totalAmount, 0);
}

void MergesortFibers::mergesortFiberFunction(std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset)
{
	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "TestSingleThreadMergeSort: start" << std::to_string(splitThreshold) << std::endl;

	if (thisAmount <= splitThreshold) {
		MergesortHelper::sortVector(sortedVector, totalAmount, thisAmount, offset);
		return;
	}

	int leftNumberAmount = thisAmount / 2;
	int leftOffset = offset;
	std::vector<int> leftSorted(leftNumberAmount);

	int rightNumberAmount = thisAmount - leftNumberAmount;
	int rightOffset = offset + leftNumberAmount;
	std::vector<int> rightSorted(rightNumberAmount);

	boost::fibers::fiber leftSplit = boost::fibers::fiber(&mergesortFiberFunction, &leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
	boost::fibers::fiber rightSplit = boost::fibers::fiber(&mergesortFiberFunction, &rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

	leftSplit.join();
	rightSplit.join();
	
	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);

	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "TestSingleThreadMergeSort: end" << std::to_string(splitThreshold) << std::endl;
}
{% endhighlight %}

Because this test benefits from parallelism, another version was made for all technologies that do not use parallelism.
This version will first create a predefined number of threads and split the work over these threads.
These threads will then call the same mergesortFiberFunction to finish their portion of the work using fibers.
This new version uses the following code:

{% highlight C++ %}
void MergesortFibers::multiThreadMergesort(int totalAmount, int splitThreshold, int maxThreadCount) {
	std::vector<int> multiThreadSortedVector{};
	int currentThreadCount = 1;
	boost::mutex threadCountMutex;

	mergesortThreadFunction(&currentThreadCount, maxThreadCount, &threadCountMutex, &multiThreadSortedVector, splitThreshold, totalAmount, totalAmount, 0);
}

void MergesortFibers::mergesortThreadFunction(int* threadCount, int maxThreadCount, boost::mutex* threadCountMutex, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset) {
	if (thisAmount <= splitThreshold) {
		MergesortHelper::sortVector(sortedVector, totalAmount, thisAmount, offset);
		return;
	}

	int leftNumberAmount = thisAmount / 2;
	int leftOffset = offset;
	std::vector<int> leftSorted(leftNumberAmount);

	int rightNumberAmount = thisAmount - leftNumberAmount;
	int rightOffset = offset + leftNumberAmount;
	std::vector<int> rightSorted(rightNumberAmount);

	bool startThread = false;
	threadCountMutex->lock();
	if (((*threadCount) + 2) <= maxThreadCount) {
		startThread = true;
		threadCount += 2;
	}
	threadCountMutex->unlock();

	if (startThread) {
		boost::thread leftSplit = boost::thread(&mergesortThreadFunction, threadCount, maxThreadCount, threadCountMutex, &leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
		boost::thread rightSplit = boost::thread(&mergesortThreadFunction, threadCount, maxThreadCount, threadCountMutex, &rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

		leftSplit.join();
		rightSplit.join();
	}
	else {
		boost::fibers::fiber leftSplit = boost::fibers::fiber(&mergesortFiberFunction, &leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
		boost::fibers::fiber rightSplit = boost::fibers::fiber(&mergesortFiberFunction, &rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

		leftSplit.join();
		rightSplit.join();
	}


	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);
}
{% endhighlight %}

The complete test code used can be found in [this GitHub repository](https://github.com/SenneBergmans/AComparisonBetweenThreadsFibersAndCoroutines).