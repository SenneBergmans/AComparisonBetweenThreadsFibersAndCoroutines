#pragma once

#include <vector>

class MergesortHelper
{
public:
	static void sortVector(std::vector<int>* sortedVector, int totalAmount, int thisAmount, int offset);
	static void combineSortedVectors(std::vector<int>* sortedVector, std::vector<int>* leftSorted, std::vector<int>* rightSorted);
	static void createParameterFiles();
};

