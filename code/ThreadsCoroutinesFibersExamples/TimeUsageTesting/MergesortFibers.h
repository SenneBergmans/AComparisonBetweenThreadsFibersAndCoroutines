#pragma once

//https://stackoverflow.com/questions/6884093/warning-c4003-not-enough-actual-parameters-for-macro-max-visual-studio-2010
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <boost/thread.hpp>
#include <boost/fiber/all.hpp>

#include <vector>
#include <string>

class MergesortFibers {
public:
	static void runMergesort(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	static void singleThreadMergesort(int totalAmount, int splitThreshold);
	static void mergesortFiberFunction(std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset);

	static void multiThreadMergesort(int totalAmount, int splitThreshold, int maxThreadCount);
	static void mergesortThreadFunction(int* threadCount, int maxThreadCount, boost::mutex* threadCountMutex, std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset);
};
