#include "ContextSwitchBoostCoroutines.h"
#include "ContextSwitchParameters.h"
#include "TimeUsage.h"
#include "BoostCoroutineHelper.h"
#include "ContextSwitchBaseTest.h"

#include <boost/bind.hpp>
#include <iostream>

const std::string ContextSwitchBoostCoroutines::_className = "BoostCoroutine";
const std::string ContextSwitchBoostCoroutines::_subClassName = "ContextSwitch";

void ContextSwitchBoostCoroutines::runContextSwitch(std::vector<std::vector<std::string>>* data) {
	ContextSwitchBaseTest::runContextSwitchTest(data, &ContextSwitchBoostCoroutines::pushContextSwitch, _className, "PushContextSwitch");
	ContextSwitchBaseTest::runContextSwitchTest(data, &ContextSwitchBoostCoroutines::pullContextSwitch, _className, "PullContextSwitch");
}

void ContextSwitchBoostCoroutines::pushContextSwitch(int input)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testPushContextSwitch " << std::to_string(input) << std::endl;

	std::vector<boost::coroutines2::coroutine<void>::push_type> coroutineVector{};

	boost::coroutines2::coroutine<void>::push_type pushCoroutine1(boost::bind(ContextSwitchBoostCoroutines::pushContextSwitchFunction, boost::placeholders::_1, input, 1));
	coroutineVector.push_back(std::move(pushCoroutine1));
	boost::coroutines2::coroutine<void>::push_type pushCoroutine2(boost::bind(ContextSwitchBoostCoroutines::pushContextSwitchFunction, boost::placeholders::_1, input, 2));
	coroutineVector.push_back(std::move(pushCoroutine2));

	BoostCoroutineHelper::joinCoroutines(&coroutineVector);
}

void ContextSwitchBoostCoroutines::pushContextSwitchFunction(boost::coroutines2::coroutine<void>::pull_type& yield, int input, int coroutineNumber)
{
	for (int i = 0; i < input; i++) {
		if (TimeUsage::showDebugPrint(input)) std::cout << coroutineNumber << " yielded" << std::endl;
		yield();
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << coroutineNumber << " ended" << std::endl;
}

void ContextSwitchBoostCoroutines::pullContextSwitch(int input)
{
	if (TimeUsage::showDebugPrint(input)) std::cout << std::endl << "starting testPullContextSwitch " << std::to_string(input) << std::endl;

	std::vector<boost::coroutines2::coroutine<void>::pull_type> coroutineVector{};

	boost::coroutines2::coroutine<void>::pull_type pullCoroutine1(boost::bind(ContextSwitchBoostCoroutines::pullContextSwitchFunction, boost::placeholders::_1, input, 1));
	coroutineVector.push_back(std::move(pullCoroutine1));
	boost::coroutines2::coroutine<void>::pull_type pullCoroutine2(boost::bind(ContextSwitchBoostCoroutines::pullContextSwitchFunction, boost::placeholders::_1, input, 2));
	coroutineVector.push_back(std::move(pullCoroutine2));

	BoostCoroutineHelper::joinCoroutines(&coroutineVector);
}

void ContextSwitchBoostCoroutines::pullContextSwitchFunction(boost::coroutines2::coroutine<void>::push_type& yield, int input, int coroutineNumber)
{
	for (int i = 0; i < input; i++) {
		if (TimeUsage::showDebugPrint(input)) std::cout << coroutineNumber << " yielded" << std::endl;
		yield();
	}

	if (TimeUsage::showDebugPrint(input)) std::cout << "Coroutine " << coroutineNumber << " ended" << std::endl;

}