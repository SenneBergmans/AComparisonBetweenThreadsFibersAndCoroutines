#include "ContextSwitchTest.h"
#include "ContextSwitchBoostCoroutines.h"
#include "ContextSwitchFibers.h"
#include "ContextSwitchCoroutinesTS.h"
#include "ContextSwitchThreads.h"

std::vector<std::vector<std::string>> ContextSwitchTest::testFunctions(std::vector<std::string> testInformation) {
	testInformation.push_back("ContextSwitchCount");
	testInformation.push_back("Sample");
	testInformation.push_back("Time");

	std::vector<std::vector<std::string>> data = { testInformation };

	std::cout << "CoroutinesTS" << std::endl;
	ContextSwitchCoroutinesTS::runContextSwitch(&data);
	std::cout << "BoostCoroutines" << std::endl;
	ContextSwitchBoostCoroutines::runContextSwitch(&data);
	std::cout << "Threads" << std::endl;
	ContextSwitchThreads::runContextSwitch(&data);
	std::cout << "Fibers" << std::endl;
	ContextSwitchFibers::runContextSwitch(&data);

	return data;
}