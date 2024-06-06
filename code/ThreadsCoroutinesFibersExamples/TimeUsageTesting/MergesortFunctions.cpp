#include "MergesortFunctions.h"
#include "MergesortHelper.h"
#include "MergesortParameters.h"
#include "TimeUsage.h"
#include "MergesortBaseTest.h"

#include <iostream>

const std::string MergesortFunctions::_className = "Functions";
const std::string MergesortFunctions::_subClassName = "Mergesort";

void MergesortFunctions::runMergesort(std::vector<std::vector<std::string>>* data) {
	MergesortBaseTest::runSingleThreadTest(data, &MergesortFunctions::singleThreadMergesort, _className);
	MergesortBaseTest::runMultiThreadTest(data, &MergesortFunctions::multiThreadMergesort, _className);
}

void MergesortFunctions::singleThreadMergesort(int totalAmount, int splitThreshold) {
	std::vector<int> singleThreadSortedVector{};
	mergesortFunction(&singleThreadSortedVector, splitThreshold, totalAmount, totalAmount, 0);
}

void MergesortFunctions::mergesortFunction(std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset) {
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

	mergesortFunction(&leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
	mergesortFunction(&rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);

	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortCoroutineFunction: end" << std::to_string(splitThreshold) << std::endl;
}

void MergesortFunctions::multiThreadMergesort(int totalAmount, int splitThreshold, int maxThreadCount) {
	std::vector<int> multiThreadSortedVector{};
	int currentThreadCount = 1;
	boost::mutex threadAmountMutex;

	mergesortThreadFunction(&currentThreadCount, maxThreadCount, &threadAmountMutex, &multiThreadSortedVector, splitThreshold, totalAmount, totalAmount, 0);
}

void MergesortFunctions::mergesortThreadFunction(int* currentThreadAmount, int maxThreadAmount, boost::mutex* threadCountMutex, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset) {
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
	if (((*currentThreadAmount) + 2) <= maxThreadAmount) {
		startThread = true;
		currentThreadAmount += 2;
	}
	threadCountMutex->unlock();

	if (startThread) {
		boost::thread leftSplit = boost::thread(&mergesortThreadFunction, currentThreadAmount, maxThreadAmount, threadCountMutex, &leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
		boost::thread rightSplit = boost::thread(&mergesortThreadFunction, currentThreadAmount, maxThreadAmount, threadCountMutex, &rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

		leftSplit.join();
		rightSplit.join();
	}
	else {
		mergesortFunction(&leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
		mergesortFunction(&rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);
	}

	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);

	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortTheadFunction: end" << std::to_string(splitThreshold) << std::endl;
}