#pragma once

#include "CoroutineHandler.h"

#include <vector>

class CoroutineTSHelper {
public:
	static void joinCoroutines(std::vector<CoroutineHandler<bool>>* coroutineVector);
};

