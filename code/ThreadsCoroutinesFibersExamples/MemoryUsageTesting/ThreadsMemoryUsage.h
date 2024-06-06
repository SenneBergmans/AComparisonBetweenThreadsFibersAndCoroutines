#pragma once

/// <summary>
/// This class contains the functions used to test the memory usage of threads.
/// </summary>
class ThreadsMemoryUsage {
public:
	ThreadsMemoryUsage() {}

	static void memoryUsage();

private:
	static void emptyMemoryUsage();
	static void parameterMemoryUsage();

	static void emptyFunction();
	static void intFunction(int intParameter);
	static void doubleFunction(double doubleParameter);
};