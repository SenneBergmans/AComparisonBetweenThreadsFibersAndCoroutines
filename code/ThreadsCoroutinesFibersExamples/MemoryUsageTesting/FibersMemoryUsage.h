#pragma once

/// <summary>
/// This class is used to measure the memory usage of fibers.
/// </summary>
class FibersMemoryUsage {
public:
	FibersMemoryUsage() {}
	static void memoryUsage();

private:
	static void emptyMemoryUsage();
	static void parameterMemoryUsage();

	static void emptyFunction();
	static void intFunction(int intParameter);
	static void doubleFunction(double doubleParameter);
};