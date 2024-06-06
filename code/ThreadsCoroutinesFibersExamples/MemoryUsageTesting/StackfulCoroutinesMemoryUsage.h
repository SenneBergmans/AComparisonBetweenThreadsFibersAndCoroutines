#pragma once
#include <boost/coroutine2/all.hpp>

/// <summary>
/// This class was used to measure the memory usage of Boost coroutines using empty functions and functions with a limited number of parameters.
/// </summary>
class StackfulCoroutinesMemoryUsage {
public:
	StackfulCoroutinesMemoryUsage() {}

	static void pushMemoryUsage();
	static void pullMemoryUsage();

private:
	static void emptyPushMemoryUsage();
	static void parameterPushMemoryUsage();
	static void returnPushMemoryUsage();

	static void emptyPushFunction(boost::coroutines2::coroutine<void>::pull_type& yield);
	static void intPushFunction(boost::coroutines2::coroutine<void>::pull_type& yield, int intParameter);
	static void doublePushFunction(boost::coroutines2::coroutine<void>::pull_type& yield, double doubleParameter);
	static void returnPushFunction(boost::coroutines2::coroutine<double>::pull_type& yield);

	static void emptyPullMemoryUsage();
	static void parameterPullMemoryUsage();
	static void returnPullMemoryUsage();

	static void emptyPullFunction(boost::coroutines2::coroutine<void>::push_type& yield);
	static void intPullFunction(boost::coroutines2::coroutine<void>::push_type& yield, int intParameter);
	static void doublePullFunction(boost::coroutines2::coroutine<void>::push_type& yield, double doubleParameter);
	static void returnPullFunction(boost::coroutines2::coroutine<double>::push_type& yield);
};