#pragma once

#include "CoroutineHandler.h"

class StacklessCoroutinesMemoryUsage
{
public:
	static void memoryUsage();

private:
	static void emptyMemoryUsage();
	static void parameterMemoryUsage();
	static void returnMemoryUsage();

	static CoroutineHandler<bool> emptyFunction();
	static CoroutineHandler<bool> oneDoubleFunction(double one);
	static CoroutineHandler<bool> twoDoublesFunction(double one, double two);
	static CoroutineHandler<bool> threeDoublesFunction(double one, double two, double three);
	static CoroutineHandler<double> returnFunction();
};

