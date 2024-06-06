#include "MergesortTest.h"
#include "MergesortCoroutinesTS.h"
#include "MergesortBoostCoroutines.h"
#include "MergesortThreads.h"
#include "MergesortFibers.h"
#include "MergesortHelper.h"
#include "MergesortFunctions.h"

#include <vector>
#include <iostream>

std::vector<std::vector<std::string>> MergesortTest::testFunctions(std::vector<std::string> testInformation) {
	testInformation.push_back("ThreadCount");
	testInformation.push_back("Sample");
	testInformation.push_back("TotalAmount");
	testInformation.push_back("SplitThreshold");
	testInformation.push_back("Time");

	std::vector<std::vector<std::string>> data = { testInformation };

	MergesortHelper::createParameterFiles();

	std::cout << "CoroutinesTS" << std::endl;
	MergesortCoroutinesTS::runMergesort(&data);
	std::cout << "BoostCoroutines" << std::endl;
	MergesortBoostCoroutines::runMergesort(&data);
	std::cout << "Threads" << std::endl;
	MergesortThreads::runMergesort(&data);
	std::cout << "Fibers" << std::endl;
	MergesortFibers::runMergesort(&data);
	std::cout << "Functions" << std::endl;
	MergesortFunctions::runMergesort(&data);

	return data;
}