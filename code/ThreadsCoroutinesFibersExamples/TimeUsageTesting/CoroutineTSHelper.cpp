#include "CoroutineTSHelper.h"

void CoroutineTSHelper::joinCoroutines(std::vector<CoroutineHandler<bool>>* coroutineVector) {
	bool stillRunning = true;

	while (stillRunning) {
		stillRunning = false;
		for (int i = 0; i < coroutineVector->size(); i++) {
			if (coroutineVector->at(i)) {
				coroutineVector->at(i)();
				stillRunning = true;
			}
		}
	}
}
