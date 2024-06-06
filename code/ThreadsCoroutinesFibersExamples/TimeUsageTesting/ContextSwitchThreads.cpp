#include "ContextSwitchThreads.h"
#include "ContextSwitchParameters.h"
#include "TimeUsage.h"
#include "ContextSwitchBaseTest.h"

#include <iostream>

const std::string ContextSwitchThreads::_className = "Thread";
const std::string ContextSwitchThreads::_subClassName = "ContextSwitch";

void ContextSwitchThreads::runContextSwitch(std::vector<std::vector<std::string>>* data) {
	ContextSwitchBaseTest::runContextSwitchTest(data, &ContextSwitchThreads::contextSwitch, _className, "ContextSwitch");
	ContextSwitchBaseTest::runContextSwitchTest(data, &ContextSwitchThreads::forceContextSwitch, _className, "ForceContextSwitch");
}

void ContextSwitchThreads::contextSwitch(int input)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testContextSwitch " << input << std::endl;

	boost::thread thread1 = boost::thread(&ContextSwitchThreads::contextSwitchFunction, input);
	boost::thread thread2 = boost::thread(&ContextSwitchThreads::contextSwitchFunction, input);

	thread1.join();
	thread2.join();
}

void ContextSwitchThreads::contextSwitchFunction(int input)
{
	for (int i = 0; i < input; i++) {
		if (TimeUsage::showDebugPrint(input)) std::cout << boost::this_thread::get_id() << " yielded " << std::endl;
		boost::this_thread::yield();
	}
}

void ContextSwitchThreads::forceContextSwitch(int input) {
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testForceContextSwitch " << input << std::endl;

	boost::mutex booleanMutex;
	bool threadSwitcher = true;

	boost::thread thread1 = boost::thread(&ContextSwitchThreads::forceContextSwitchFunction, input, &booleanMutex, &threadSwitcher, true);
	boost::thread thread2 = boost::thread(&ContextSwitchThreads::forceContextSwitchFunction, input, &booleanMutex, &threadSwitcher, false);

	thread1.join();
	thread2.join();
}

void ContextSwitchThreads::forceContextSwitchFunction(int input, boost::mutex* booleanMutex, bool* threadSwitcher, bool switchState) {
	int counter = 0;
	while (counter < input)
	{
		boost::lock_guard<boost::mutex> guard(*booleanMutex);
		if (*threadSwitcher == switchState) {
			counter++;
			*threadSwitcher = !switchState;
			if (TimeUsage::showDebugPrint(input)) std::cout << boost::this_thread::get_id() << " yielded " << std::endl;
		}
		boost::this_thread::yield();
	}
}