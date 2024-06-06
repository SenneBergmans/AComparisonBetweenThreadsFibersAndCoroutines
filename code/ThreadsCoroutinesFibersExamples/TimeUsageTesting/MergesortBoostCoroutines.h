#pragma once

#include <boost/coroutine2/all.hpp>
#include <boost/thread.hpp>

#include <vector>
#include <string>

class MergesortBoostCoroutines {
public:
	static void runMergesort(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	static void singleThreadMergesort(int totalAmount, int splitThreshold);
	static void mergesortCoroutineFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset);

	static void multiThreadMergesort(int totalAmount, int splitThreshold, int maxThreadCount);
	static void mergesortThreadFunction(int* threadCount, int maxThreadCount, boost::mutex* threadCountMutex, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset);
};

