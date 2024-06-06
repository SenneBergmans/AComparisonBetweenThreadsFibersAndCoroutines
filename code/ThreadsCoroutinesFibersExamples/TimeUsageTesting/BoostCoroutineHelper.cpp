#include "BoostCoroutineHelper.h"

void BoostCoroutineHelper::joinCoroutines(std::vector<boost::coroutines2::coroutine<void>::push_type>* coroutineVector) {
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

void BoostCoroutineHelper::joinCoroutines(std::vector<boost::coroutines2::coroutine<void>::pull_type>* coroutineVector) {
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