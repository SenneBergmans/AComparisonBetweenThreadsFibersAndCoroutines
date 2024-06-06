#pragma once

#include <vector>
#include <string>

class ContextSwitchFibers
{
public:
	/// <summary>
	/// Run all context switch tests using fibers.
	/// 
	/// The test is contextSwitch
	/// </summary>
	/// <param name="data">The vector to which the test results will be appended.</param>
	static void runContextSwitch(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	/// <summary>
	/// Create two fibers and have each of them yield 'input' times.
	/// </summary>
	/// <param name="input">The amount of times each fiber will yield.</param>
	static void contextSwitch(int input);

	/// <summary>
	/// A function that will yield its fiber 'input' times before ending
	/// </summary>
	/// <param name="input">The amount of times the fiber will yield.</param>
	static void contextSwitchFunction(int input);
};

