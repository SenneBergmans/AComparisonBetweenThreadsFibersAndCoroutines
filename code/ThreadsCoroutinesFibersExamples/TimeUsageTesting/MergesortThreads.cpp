#include "MergesortThreads.h"
#include "MergesortHelper.h"
#include "MergesortParameters.h"
#include "TimeUsage.h"
#include "MergesortBaseTest.h"

//https://stackoverflow.com/questions/6884093/warning-c4003-not-enough-actual-parameters-for-macro-max-visual-studio-2010
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>

const std::string MergesortThreads::_className = "Thread";
const std::string MergesortThreads::_subClassName = "Mergesort";

void MergesortThreads::runMergesort(std::vector<std::vector<std::string>>* data)
{
	MergesortBaseTest::runSingleThreadTest(data, &MergesortThreads::threadMergesort, _className);
}

void MergesortThreads::threadMergesort(int totalAmount, int splitThreshold) {
	std::vector<int> threadsSortedVector{};
	mergesortThreadFunction(&threadsSortedVector, splitThreshold, totalAmount, totalAmount, 0);
}

void MergesortThreads::mergesortThreadFunction(std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset)
{
	if(TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortThreadFunction: start" << std::to_string(splitThreshold) << std::endl;

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

	boost::thread leftSplit = boost::thread(&mergesortThreadFunction, &leftSorted, splitThreshold, totalAmount, leftNumberAmount, leftOffset);
	boost::thread rightSplit = boost::thread(&mergesortThreadFunction, &rightSorted, splitThreshold, totalAmount, rightNumberAmount, rightOffset);

	leftSplit.join();
	rightSplit.join();

	MergesortHelper::combineSortedVectors(sortedVector, &leftSorted, &rightSorted);

	if (TimeUsage::showDebugPrint(splitThreshold)) std::cout << std::endl << "MergesortThreadFunction: end" << std::to_string(splitThreshold) << std::endl;
}
