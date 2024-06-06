#pragma once

#include <vector>
#include <string>
#include <boost/coroutine2/all.hpp>

class ContextSwitchBoostCoroutines
{
public:
	/// <summary>
	/// Run all context switch tests using boost coroutines.
	/// 
	/// The tests are: pushContextSwitch, pullContextSwitch
	/// </summary>
	/// <param name="data">The vector to which the test results will be appended.</param>
	static void runContextSwitch(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	/// <summary>
	/// Create two push type coroutines and have each of them yield 'input' times.
	/// </summary>
	/// <param name="input">The amount of times each coroutine will yield.</param>
	static void pushContextSwitch(int input);

	/// <summary>
	/// A push type coroutine that will yield 'input' times before ending
	/// </summary>
	/// <param name="yield">The pull type coroutine used to yield.</param>
	/// <param name="input">The amount of times the coroutine will yield before ending.</param>
	/// <param name="coroutineNumber">The identification number of the coroutine to help visualisation in debugPrint.</param>
	static void pushContextSwitchFunction(boost::coroutines2::coroutine<void>::pull_type& yield, int input, int coroutineNumber);
	
	/// <summary>
	/// Create two pull type coroutines and have each of them yield 'input' times.
	/// </summary>
	/// <param name="input">The amount of times each coroutine will yield.</param>
	static void pullContextSwitch(int input);

	/// <summary>
	/// A pull type coroutine that will yield 'input' times before ending
	/// </summary>
	/// <param name="yield">The push type coroutine used to yield.</param>
	/// <param name="input">The amount of times the coroutine will yield before ending.</param>
	/// <param name="coroutineNumber">The identification number of the coroutine to help visualisation in debugPrint.</param>
	static void pullContextSwitchFunction(boost::coroutines2::coroutine<void>::push_type& yield, int input, int coroutineNumber);

};

