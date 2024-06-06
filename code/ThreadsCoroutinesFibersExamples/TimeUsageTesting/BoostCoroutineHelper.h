#pragma once

#include <boost/coroutine2/all.hpp>
#include <vector>

class BoostCoroutineHelper {
public:
	static void joinCoroutines(std::vector<boost::coroutines2::coroutine<void>::push_type>* coroutineVector);
	
	static void joinCoroutines(std::vector<boost::coroutines2::coroutine<void>::pull_type>* coroutineVector);
};