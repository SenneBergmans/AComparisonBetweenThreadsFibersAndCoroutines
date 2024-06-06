#include "StackfulCoroutinesMemoryUsage.h"
#include "StacklessCoroutinesMemoryUsage.h"
#include "FibersMemoryUsage.h"
#include "ThreadsMemoryUsage.h"

#include <iostream>

int main()
{
	StackfulCoroutinesMemoryUsage().pushMemoryUsage();
	StackfulCoroutinesMemoryUsage().pullMemoryUsage();
	StacklessCoroutinesMemoryUsage().memoryUsage();
	FibersMemoryUsage().memoryUsage();
	ThreadsMemoryUsage().memoryUsage();
}