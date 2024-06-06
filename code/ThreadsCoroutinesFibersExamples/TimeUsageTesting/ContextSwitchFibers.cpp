#include "ContextSwitchFibers.h"
#include "ContextSwitchParameters.h"
#include "TimeUsage.h"
#include "ContextSwitchBaseTest.h"

//https://stackoverflow.com/questions/6884093/warning-c4003-not-enough-actual-parameters-for-macro-max-visual-studio-2010
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>
#include <boost/bind.hpp>
#include <iostream>

const std::string ContextSwitchFibers::_className = "Fibers";
const std::string ContextSwitchFibers::_subClassName = "ContextSwitch";

void ContextSwitchFibers::runContextSwitch(std::vector<std::vector<std::string>>* data)
{
	ContextSwitchBaseTest::runContextSwitchTest(data, &ContextSwitchFibers::contextSwitch, _className, "ContextSwitch");
}

void ContextSwitchFibers::contextSwitch(int input)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testContextSwitch " << std::to_string(input) << std::endl;

	boost::fibers::fiber fiber1 = boost::fibers::fiber(&ContextSwitchFibers::contextSwitchFunction, input);
	boost::fibers::fiber fiber2 = boost::fibers::fiber(&ContextSwitchFibers::contextSwitchFunction, input);

	fiber1.join();
	fiber2.join();
}

void ContextSwitchFibers::contextSwitchFunction(int input)
{
	for (int i = 0; i < input; i++) {
		if (TimeUsage::showDebugPrint(input)) std::cout << boost::this_fiber::get_id() << " yielded " << std::endl;
		boost::this_fiber::yield();
	}
}