#include "ContextSwitchCoroutinesTS.h"
#include "ContextSwitchParameters.h"
#include "TimeUsage.h"
#include "CoroutineTSHelper.h"
#include "ContextSwitchBaseTest.h"

#include <boost/bind.hpp>
#include <iostream>

const std::string ContextSwitchCoroutinesTS::_className = "CoroutineTS";
const std::string ContextSwitchCoroutinesTS::_subClassName = "ContextSwitch";

void ContextSwitchCoroutinesTS::runContextSwitch(std::vector<std::vector<std::string>>* data)
{
	ContextSwitchBaseTest::runContextSwitchTest(data, &ContextSwitchCoroutinesTS::contextSwitch, _className, "ContextSwitch");
}

void ContextSwitchCoroutinesTS::contextSwitch(int input)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testContextSwitch " << std::to_string(input) << std::endl;

	std::vector<CoroutineHandler<bool>> coroutineVector{};

	coroutineVector.push_back(contextSwitchFunction(input, 1));
	coroutineVector.push_back(contextSwitchFunction(input, 2));

	CoroutineTSHelper::joinCoroutines(&coroutineVector);
}

CoroutineHandler<bool> ContextSwitchCoroutinesTS::contextSwitchFunction(int input, int coroutineNumber)
{
	for (int i = 0; i < input; i++) {
		if (TimeUsage::showDebugPrint(input)) std::cout << coroutineNumber << " yielded" << std::endl;
		co_yield true;
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << coroutineNumber << " ended" << std::endl;
	co_return;
}