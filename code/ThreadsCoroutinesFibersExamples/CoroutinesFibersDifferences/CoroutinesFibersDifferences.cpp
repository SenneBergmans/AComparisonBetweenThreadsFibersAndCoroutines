//https://stackoverflow.com/questions/6884093/warning-c4003-not-enough-actual-parameters-for-macro-max-visual-studio-2010
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>
#include <boost/coroutine2/all.hpp>

//The two test functions do the same thing. When called, they print the number that was provided and yield.
//When called again at a later time, they resume and print their number again and then complete.
void fiberFunction(int number) {
	std::cout << number << " ";
	boost::this_fiber::yield();
	std::cout << number << " ";
}

//The added pull_type parameter is used by the coroutine to yield.
//This parameter does not affect the inner workings of the function.
void coroutineFunction(boost::coroutines2::coroutine<void>::pull_type& yield, int number) {
	std::cout << number << " ";
	yield();
	std::cout << number << " ";
}

void fiberJoin() {
	std::cout << "FiberJoin: ";
	boost::fibers::fiber fiber1(fiberFunction, 1);
	fiber1.join();
	std::cout << std::endl;
}

void coroutineJoin() {
	std::cout << "CoroutineJoin: ";
	boost::coroutines2::coroutine<void>::push_type coroutine1(std::bind(&coroutineFunction, std::placeholders::_1, 1));
	while (coroutine1) {
		coroutine1();
	}
	std::cout << std::endl;
}

void multiFiberJoin() {
	std::cout << "MultiFiberJoin: ";
	boost::fibers::fiber fiber1(fiberFunction, 1);
	boost::fibers::fiber fiber2(fiberFunction, 2);

	fiber1.join();
	fiber2.join();
	std::cout << std::endl;
}

void oneByOneCoroutineJoin() {
	std::cout << "OneByOneCoroutineJoin: ";
	boost::coroutines2::coroutine<void>::push_type coroutine1(std::bind(&coroutineFunction, std::placeholders::_1, 1));
	boost::coroutines2::coroutine<void>::push_type coroutine2(std::bind(&coroutineFunction, std::placeholders::_1, 2));
	while (coroutine1) {
		coroutine1();
	}
	while (coroutine2) {
		coroutine2();
	}
	std::cout << std::endl;
}

void alternatingCoroutineJoin() {
	std::cout << "AlternatingCoroutineJoin: ";
	boost::coroutines2::coroutine<void>::push_type coroutine1(std::bind(&coroutineFunction, std::placeholders::_1, 1));
	boost::coroutines2::coroutine<void>::push_type coroutine2(std::bind(&coroutineFunction, std::placeholders::_1, 2));
	
	bool finished = false;
	while (!finished) {
		finished = true;
		if (coroutine1) { 
			coroutine1();
			finished = false;
		}
		if (coroutine2) {
			coroutine2();
			finished = false;
		}
	}
	std::cout << std::endl;
}

void usageExample() {
	fiberJoin();
	coroutineJoin();
	multiFiberJoin();
	oneByOneCoroutineJoin();
	alternatingCoroutineJoin();

	// --- Output ---  
	// FiberJoin: 1 1
	// CoroutineJoin : 1 1
	// MultiFiberJoin : 1 2 1 2
	// OneByOneCoroutineJoin : 1 1 2 2
	// AlternatingCoroutineJoin : 1 2 1 2
	// --- Output ---
}

void fiberSchedulerFunction(std::string text) {
	std::cout << text << " ";
}

void coroutineSchedulerFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::string text) {
	std::cout << text << " ";
}

void subFiber() {
	boost::fibers::fiber fiber2(fiberSchedulerFunction, "sub");
	fiber2.join();
}

void mainFiber() {
	std::cout << "MainFiber: ";
	boost::fibers::fiber fiber1(fiberSchedulerFunction, "main");
	subFiber();
	fiber1.join();
	std::cout << std::endl;
}

void subCoroutine() {
	boost::coroutines2::coroutine<void>::push_type coroutine2(std::bind(coroutineSchedulerFunction, std::placeholders::_1, "sub"));
	coroutine2();
}

void mainCoroutine() {
	std::cout << "MainCoroutine: ";
	boost::coroutines2::coroutine<void>::push_type coroutine1(std::bind(coroutineSchedulerFunction, std::placeholders::_1, "main"));
	subCoroutine();
	coroutine1();
	std::cout << std::endl;
}

void schedulerExample() {
	mainFiber();
	mainCoroutine();

	// --- Output ---  
	// MainFiber: main sub
	// MainCoroutine: sub main
	// --- Output ---
}
int main() {
	usageExample();
	std::cout << std::endl;
	schedulerExample();
}