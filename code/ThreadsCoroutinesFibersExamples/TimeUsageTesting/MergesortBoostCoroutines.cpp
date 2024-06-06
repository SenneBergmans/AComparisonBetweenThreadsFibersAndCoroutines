#include "MergesortBoostCoroutines.h"
#include "MergesortHelper.h"
#include "MergesortParameters.h"
#include "TimeUsage.h"
#include "MergesortBaseTest.h"

#include <iostream>

const std::string MergesortBoostCoroutines::_className = "BoostCoroutine";
const std::string MergesortBoostCoroutines::_subClassName = "Mergesort";

void MergesortBoostCoroutines::runMergesort(std::vector<std::vector<std::string>>* data)
{
	MergesortBaseTest::runSingleThreadTest(data, &MergesortBoostCoroutines::singleThreadMergesort, _className);
	MergesortBaseTest::runMultiThreadTest(data, &MergesortBoostCoroutines::multiThreadMergesort, _className);
}

void MergesortBoostCoroutines::singleThreadMergesort(int totalAmount, int splitThreshold) {
	std::vector<int> singleThreadSortedVector{};
	boost::coroutines2::coroutine<void>::push_type startingCoroutine(boost::bind(&MergesortBoostCoroutines::mergesortCoroutineFunction, boost::placeholders::_1, &singleThreadSortedVector, splitThreshold, totalAmount, totalAmount, 0));
	
	startingCoroutine();
}

void MergesortBoostCoroutines::mergesortCoroutineFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset)
{
	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortCoroutineFunction: start" << std::to_string(splitThreshold) << std::endl;

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

	boost::coroutines2::coroutine<void>::push_type leftSplit(boost::bind(MergesortBoostCoroutines::mergesortCoroutineFunction, boost::placeholders::_1, &leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset));
	boost::coroutines2::coroutine<void>::push_type rightSplit(boost::bind(MergesortBoostCoroutines::mergesortCoroutineFunction, boost::placeholders::_1, &rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset));
	
	leftSplit();
	rightSplit();

	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);

	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortCoroutineFunction: end" << std::to_string(splitThreshold) << std::endl;
}

void MergesortBoostCoroutines::multiThreadMergesort(int totalAmount, int splitThreshold, int maxThreadCount) {
	std::vector<int> multiThreadSortedVector{};
	int currentThreadCount = 1;
	boost::mutex threadCountMutex;

	mergesortThreadFunction(&currentThreadCount, maxThreadCount, &threadCountMutex, &multiThreadSortedVector, splitThreshold, totalAmount, totalAmount, 0);
}

void MergesortBoostCoroutines::mergesortThreadFunction(int* currentThreadCount, int maxThreadCount, boost::mutex* threadCountMutex, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset)
{
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
	if (((*currentThreadCount) + 2) <= maxThreadCount) {
		startThread = true;
		currentThreadCount += 2;
	}
	threadCountMutex->unlock();

	if (startThread) {
		boost::thread leftSplit = boost::thread(&mergesortThreadFunction, currentThreadCount, maxThreadCount, threadCountMutex, &leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
		boost::thread rightSplit = boost::thread(&mergesortThreadFunction, currentThreadCount, maxThreadCount, threadCountMutex, &rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

		leftSplit.join();
		rightSplit.join();
	}
	else {
		boost::coroutines2::coroutine<void>::push_type leftSplit(boost::bind(MergesortBoostCoroutines::mergesortCoroutineFunction, boost::placeholders::_1, &leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset));
		boost::coroutines2::coroutine<void>::push_type rightSplit(boost::bind(MergesortBoostCoroutines::mergesortCoroutineFunction, boost::placeholders::_1, &rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset));

		leftSplit();
		rightSplit();
	}

	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);

	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "TestSingleThreadMergeSort: end" << std::to_string(splitThreshold) << std::endl;
}