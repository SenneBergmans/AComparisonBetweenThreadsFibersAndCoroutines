#pragma once

#include "CoroutineHandler.h"

#include <coroutine>
#include <boost/thread.hpp>
#include <vector>
#include <string>

class MergesortCoroutinesTS {
public: 
	static void runMergesort(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	static void singleThreadMergesort(int totalAmount, int splitThreshold);
	static CoroutineHandler<bool> mergesortCoroutineFunction(std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset);

	static void multiThreadMergesort(int totalAmount, int splitThreshold, int maxThreadCount);
	static void mergesortThreadFunction(int* threadCount, int maxThreadCount, boost::mutex* threadCountMutex, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset);
};

