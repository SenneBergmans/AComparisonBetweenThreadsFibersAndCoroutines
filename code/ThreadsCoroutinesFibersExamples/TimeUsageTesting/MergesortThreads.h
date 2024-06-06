#pragma once

#include <vector>
#include <string>

class MergesortThreads
{
public:
	static void runMergesort(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	static void threadMergesort(int totalAmount, int splitThreshold);
	static void mergesortThreadFunction(std::vector<int>* sortedVector, int splitThreshold, int totalAmount, int thisAmount, int offset);
};
