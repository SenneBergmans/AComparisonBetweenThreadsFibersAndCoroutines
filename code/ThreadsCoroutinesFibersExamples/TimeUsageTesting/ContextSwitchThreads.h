#pragma once

#include <vector>
#include <string>
#include <boost/thread/thread.hpp>

class ContextSwitchThreads
{
public:
	/// <summary>
	/// Run all context switch tests using threads.
	/// 
	/// The tests are contextSwitch, forceContextSwitch.
	/// </summary>
	/// <param name="data">The vector to which the test results will be appended.</param>
	static void runContextSwitch(std::vector<std::vector<std::string>>* data);

private:
	static const std::string _className;
	static const std::string _subClassName;

	/// <summary>
	/// Create two threads and have each of them try to yield 'input' times.
	/// </summary>
	/// <param name="input">The amount of times each thread will try to yield.</param>
	static void contextSwitch(int input);

	/// <summary>
	/// A function that will yield the thread 'input' times
	/// </summary>
	/// <param name="input">The amount of times the thread will try to yield.</param>
	static void contextSwitchFunction(int input);

	/// <summary>
	/// Create two threads and have each of them yield 'input' times.
	/// </summary>
	/// <param name="input">The amount of times each thread will yield.</param>
	static void forceContextSwitch(int input);

	/// <summary>
	/// A function that will yield the thread 'input' times
	/// </summary>
	/// <param name="input">The amount of times the thread will yield.</param>
	/// <param name="booleanMutex">A mutex to only allow one of the threads to change threadSwitcher.</param>
	/// <param name="threadSwitcher">A boolean forcing the two threads to switch before continuing.</param>
	/// <param name="switchState">The boolean state in which this thread can continue.</param>
	static void forceContextSwitchFunction(int input, boost::mutex* booleanMutex, bool* threadSwitcher, bool switchState);
};

