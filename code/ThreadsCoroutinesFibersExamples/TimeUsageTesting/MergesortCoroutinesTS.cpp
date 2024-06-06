#include "MergesortCoroutinesTS.h"
#include "MergesortHelper.h"
#include "MergesortParameters.h"
#include "TimeUsage.h"
#include "MergesortBaseTest.h"
#include "CoroutineHandler.h"

const std::string MergesortCoroutinesTS::_className = "CoroutineTS";
const std::string MergesortCoroutinesTS::_subClassName = "Mergesort";

void MergesortCoroutinesTS::runMergesort(std::vector<std::vector<std::string>>* data) {
	MergesortBaseTest::runSingleThreadTest(data, &MergesortCoroutinesTS::singleThreadMergesort, _className);
	MergesortBaseTest::runMultiThreadTest(data, &MergesortCoroutinesTS::multiThreadMergesort, _className);
}

void MergesortCoroutinesTS::singleThreadMergesort(int totalAmount, int splitThreshold) {
	std::vector<int> singleThreadSortedVector{};
	CoroutineHandler<bool> startingCoroutine = mergesortCoroutineFunction(&singleThreadSortedVector, splitThreshold, totalAmount, totalAmount, 0);

	startingCoroutine();
}

CoroutineHandler<bool> MergesortCoroutinesTS::mergesortCoroutineFunction(std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset) {
	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortCoroutineFunction: start" << std::to_string(splitThreshold) << std::endl;

	if (thisAmount <= splitThreshold) {
		MergesortHelper::sortVector(sortedVector, totalAmount, thisAmount, offset);
		co_return;
	}

	int leftNumberAmount = thisAmount / 2;
	int leftOffset = offset;
	std::vector<int> leftSorted(leftNumberAmount);

	int rightNumberAmount = thisAmount - leftNumberAmount;
	int rightOffset = offset + leftNumberAmount;
	std::vector<int> rightSorted(rightNumberAmount);

	CoroutineHandler<bool> leftSplit = mergesortCoroutineFunction(&leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
	CoroutineHandler<bool> rightSplit = mergesortCoroutineFunction(&rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

	leftSplit();
	rightSplit();

	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);

	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortCoroutineFunction: end" << std::to_string(splitThreshold) << std::endl;
	co_return;
}

void MergesortCoroutinesTS::multiThreadMergesort(int totalAmount, int splitThreshold, int maxThreadCount) {
	std::vector<int> multiThreadSortedVector{};
	int currentThreadCount = 1;
	boost::mutex threadAmountMutex;

	mergesortThreadFunction(&currentThreadCount, maxThreadCount, &threadAmountMutex, &multiThreadSortedVector, splitThreshold, totalAmount, totalAmount, 0);
}

void MergesortCoroutinesTS::mergesortThreadFunction(int* currentThreadAmount, int maxThreadAmount, boost::mutex* threadCountMutex, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset) {
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
		CoroutineHandler<bool> leftSplit = mergesortCoroutineFunction(&leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
		CoroutineHandler<bool> rightSplit = mergesortCoroutineFunction(&rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

		leftSplit();
		rightSplit();
	}

	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);

	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortTheadFunction: end" << std::to_string(splitThreshold) << std::endl;
}