#include "ThreadsMemoryUsage.h"

//https://stackoverflow.com/questions/6884093/warning-c4003-not-enough-actual-parameters-for-macro-max-visual-studio-2010
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <boost/thread.hpp>

#include <iostream>

void ThreadsMemoryUsage::memoryUsage() {
	do {} while (0);
	//std::cout << std::endl << "Before emptyMemoryUsage" << std::endl;
	emptyMemoryUsage();
	do {} while (0);
	//std::cout << "After emptyMemoryUsage" << std::endl;

	do {} while (0);
	//std::cout << std::endl << "Before parameterMemoryUsage" << std::endl;
	parameterMemoryUsage();
	do {} while (0);
	//std::cout << "After parameterMemoryUsage" << std::endl;
}

void ThreadsMemoryUsage::emptyMemoryUsage() {
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: start" << std::endl;

	boost::thread thread_1(emptyFunction);
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: after creation thread_1" << std::endl;

	boost::thread thread_2(emptyFunction);
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: after creation thread_2" << std::endl;

	boost::thread thread_3(emptyFunction);
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: after creation thread_3" << std::endl;

	thread_1.join();
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: thread_1 joined" << std::endl;
	thread_2.join();
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: thread_2 joined" << std::endl;
	thread_3.join();
	do {} while (0);
	//std::cout << "EmptyMemoryUsage: thread_3 joined" << std::endl;

	do {} while (0);
	//std::cout << "EmptyMemoryUsage: end" << std::endl;
}

void ThreadsMemoryUsage::emptyFunction() {}

void ThreadsMemoryUsage::parameterMemoryUsage() {
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: start" << std::endl;

	boost::thread thread_1_int(intFunction, 1);
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: after creation thread_1_int" << std::endl;

	boost::thread thread_2_double(doubleFunction, 2.1);
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: after creation thread_2_double" << std::endl;

	boost::thread thread_3_double(doubleFunction, 3.1);
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: after creation thread_3_double" << std::endl;

	thread_1_int.join();
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: thread_1_int joined" << std::endl;
	thread_2_double.join();
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: thread_2_double joined" << std::endl;
	thread_3_double.join();
	do {} while (0);
	//std::cout << "ParameterMemoryUsage: thread_3_double joined" << std::endl;

	do {} while (0);
	//std::cout << "ParameterMemoryUsage: end" << std::endl;
}

void ThreadsMemoryUsage::intFunction(int intParameter) {
	do {} while (0);
	//std::cout << "IntFunction: started with " << intParameter << std::endl;

	do {} while (0);
	//std::cout << "IntFunction: end" << std::endl;
}

void ThreadsMemoryUsage::doubleFunction(double doubleParameter) {
	do {} while (0);
	//std::cout << "DoubleFunction: started with " << doubleParameter << std::endl;

	do {} while (0);
	//std::cout << "DoubleFunction: end" << std::endl;
}