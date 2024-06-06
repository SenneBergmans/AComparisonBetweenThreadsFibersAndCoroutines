#include "MergesortFibers.h"
#include "MergesortHelper.h"
#include "MergesortParameters.h"
#include "TimeUsage.h"
#include "MergesortBaseTest.h"

#include <boost/bind.hpp>
#include <iostream>

const std::string MergesortFibers::_className = "Fiber";
const std::string MergesortFibers::_subClassName = "Mergesort";

void MergesortFibers::runMergesort(std::vector<std::vector<std::string>>* data)
{
	MergesortBaseTest::runSingleThreadTest(data, &MergesortFibers::singleThreadMergesort, _className);
	MergesortBaseTest::runMultiThreadTest(data, &MergesortFibers::multiThreadMergesort, _className);
}

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