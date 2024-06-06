#pragma once

#include "CoroutineHandler.h"

#include <vector>
#include <string>
#include <coroutine>

class ContextSwitchCoroutinesTS
{
public:
	/// <summary>
	/// Run all context switch tests using coroutines TS.
	/// 
	/// The tests is contextSwitch
	/// </summary>
	/// <param name="data">The vector to which the test results will be appended.</param>
	static void runContextSwitch(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	/// <summary>
	/// Create two coroutines and have each of them yield 'input' times.
	/// </summary>
	/// <param name="input">The amount of times each coroutine will yield.</param>
	static void contextSwitch(int input);

	/// <summary>
	/// A coroutine that will yield 'input' times before ending
	/// </summary>
	/// <param name="input">The amount of times the coroutine will yield before ending.</param>
	/// <param name="coroutineNumber">The identification number of the coroutine to help visualisation in debugPrint.</param>
	static CoroutineHandler<bool> contextSwitchFunction(int input, int coroutineNumber);
};

