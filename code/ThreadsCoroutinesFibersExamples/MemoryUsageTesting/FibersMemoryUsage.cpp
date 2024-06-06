#include "FibersMemoryUsage.h"

//https://stackoverflow.com/questions/6884093/warning-c4003-not-enough-actual-parameters-for-macro-max-visual-studio-2010
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>

#include <iostream>

void FibersMemoryUsage::memoryUsage() {
	do {} while (0);
	//std::cout << std::endl << "Before emptyTSMemoryUsage" << std::endl;
	emptyMemoryUsage();
	do {} while (0);
	//std::cout << "After emptyTSMemoryUsage" << std::endl;

	do {} while (0);
	//std::cout << std::endl << "Before parameterTSMemoryUsage" << std::endl;
	parameterMemoryUsage();
	do {} while (0);
	//std::cout << "After parameterTSMemoryUsage" << std::endl;
}

void FibersMemoryUsage::emptyMemoryUsage() {
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: start" << std::endl;
	boost::fibers::fiber fiber_1(emptyFunction);
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: after creation fiber_1" << std::endl;

	boost::fibers::fiber fiber_2(emptyFunction);
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: after creation fiber_2" << std::endl;

	boost::fibers::fiber fiber_3(emptyFunction);
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: after creation fiber_3" << std::endl;

	fiber_1.join();
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: fiber_2 joined" << std::endl;
	fiber_2.join();
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: fiber_1 joined" << std::endl;
	fiber_3.join();
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: fiber_3 joined" << std::endl;

	do {} while (0);
	//std::cout << "EmptyMemoryUsage: end" << std::endl;
}

void FibersMemoryUsage::emptyFunction() {
	boost::this_fiber::yield();
}

void FibersMemoryUsage::parameterMemoryUsage() {
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: start" << std::endl;

	boost::fibers::fiber fiber_1_int(intFunction, 1);
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: after creation fiber_1" << std::endl;

	boost::fibers::fiber fiber_2_double(doubleFunction, 2.1);
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: after creation fiber_2" << std::endl;

	boost::fibers::fiber fiber_3_double(doubleFunction, 3.1);
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: after creation fiber_3" << std::endl;

	fiber_1_int.join();
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: fiber_1_int joined" << std::endl;
	
	fiber_2_double.join();
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: fiber_2_double joined" << std::endl;
	
	fiber_3_double.join();
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: fiber_3_double joined" << std::endl;

	do {} while (0);
	//std::cout << "ParameterMemoryUsage: end" << std::endl;
}

void FibersMemoryUsage::intFunction(int intParameter) {
	//std::cout << "IntFunction: started with " << intParameter << std::endl;
	do {} while (0);
	//std::cout << "IntFunction: end" << std::endl;
}

void FibersMemoryUsage::doubleFunction(double doubleParameter) {
	//std::cout << "DoubleFunction: started with " << doubleParameter << std::endl;
	do {} while (0);
	//std::cout << "DoubleFunction: end" << std::endl;
}