#include "StackfulCoroutinesMemoryUsage.h"

#include <boost/bind.hpp>
#include <iostream>

void StackfulCoroutinesMemoryUsage::pushMemoryUsage()
{

	//do {} while (0); lines are used to trigger breakpoints at the right places in the code
	//They replace the previous std::cout because using std::cout inside of the functions seemed to use dynamic memory at some points in the code
	//To measure only the memory usage of the coroutines they are replaced.
	do {} while (0);
	//std::cout << std::endl << "Before emptyPushMemoryUsage" << std::endl;
	emptyPushMemoryUsage();
	do {} while (0);
	//std::cout << "After emptyPushMemoryUsage" << std::endl;

	do {} while (0);
	//std::cout << std::endl << "Before parameterPushMemoryUsage" << std::endl;
	parameterPushMemoryUsage();
	do {} while (0);
	//std::cout << "After parameterPushMemoryUsage" << std::endl;

	do {} while (0);
	//std::cout << std::endl << "Before returnPushMemoryUsage" << std::endl;
	returnPushMemoryUsage();
	do {} while (0);
	//std::cout << "After returnPushMemoryUsage" << std::endl;
}

void StackfulCoroutinesMemoryUsage::emptyPushMemoryUsage()
{
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: start" << std::endl;

	boost::coroutines2::coroutine<void>::push_type push_1(boost::bind(emptyPushFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after creation push_1" << std::endl;

	boost::coroutines2::coroutine<void>::push_type push_2(boost::bind(emptyPushFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after creation push_2" << std::endl;

	boost::coroutines2::coroutine<void>::push_type push_3(boost::bind(emptyPushFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after creation push_3" << std::endl;


	push_1();
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after first call push_1" << std::endl;

	push_2();
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after first call push_2" << std::endl;

	push_3();
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after first call push_3" << std::endl;

	push_1();
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after second call push_1" << std::endl;

	push_2();
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after second call push_2" << std::endl;

	push_3();
	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: after second call push_3" << std::endl;

	do {} while (0);
	//std::cout << "EmptyPushMemoryUsage: end" << std::endl;
}

void StackfulCoroutinesMemoryUsage::emptyPushFunction(boost::coroutines2::coroutine<void>::pull_type& yield)
{
	yield();
}

void StackfulCoroutinesMemoryUsage::parameterPushMemoryUsage()
{
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: start" << std::endl;

	boost::coroutines2::coroutine<void>::push_type push_1_int(boost::bind(intPushFunction, boost::placeholders::_1, 1));
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after creation push_1_int" << std::endl;

	boost::coroutines2::coroutine<void>::push_type push_2_double(boost::bind(doublePushFunction, boost::placeholders::_1, 2.1));
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after creation push_2_double" << std::endl;

	boost::coroutines2::coroutine<void>::push_type push_3_double(boost::bind(doublePushFunction, boost::placeholders::_1, 3.1));
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after creation push_3_double" << std::endl;


	push_1_int();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after first call push_1_int" << std::endl;

	push_2_double();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after first call push_2_double" << std::endl;

	push_3_double();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after first call push_3_double" << std::endl;

	push_1_int();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after second call push_1_int" << std::endl;

	push_2_double();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after second call push_2_double" << std::endl;

	push_3_double();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after second call push_3_double" << std::endl;

	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: end" << std::endl;
}

void StackfulCoroutinesMemoryUsage::intPushFunction(boost::coroutines2::coroutine<void>::pull_type& yield, int intParameter)
{
	yield();
}

void StackfulCoroutinesMemoryUsage::doublePushFunction(boost::coroutines2::coroutine<void>::pull_type& yield, double doubleParameter)
{
	yield();
}

void StackfulCoroutinesMemoryUsage::returnPushMemoryUsage() {
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: start" << std::endl;

	boost::coroutines2::coroutine<double>::push_type push_1(boost::bind(returnPushFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after creation push_1" << std::endl;

	boost::coroutines2::coroutine<double>::push_type push_2(boost::bind(returnPushFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after creation push_2" << std::endl;

	boost::coroutines2::coroutine<double>::push_type push_3(boost::bind(returnPushFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after creation push_3" << std::endl;


	push_1(1.1);
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after first call push_1" << std::endl;

	push_2(2.1);
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after first call push_2" << std::endl;

	push_3(3.1);
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after first call push_3" << std::endl;

	push_1(1.2);
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after second call push_1" << std::endl;

	push_2(2.2);
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after second call push_2" << std::endl;

	push_3(3.2);
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after second call push_3" << std::endl;

	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: end" << std::endl;
}

void StackfulCoroutinesMemoryUsage::returnPushFunction(boost::coroutines2::coroutine<double>::pull_type& yield) {
	double input = yield.get();
	do {} while (0);
	//std::cout << "ReturnPushFunction: first call with input " << input << std::endl;
	yield();
	input = yield.get();
	do {} while (0);
	//std::cout << "ReturnPushFunction: second call with input " << input << std::endl;
}

void StackfulCoroutinesMemoryUsage::pullMemoryUsage()
{
	do {} while (0);
	//std::cout << std::endl << "Before emptyPullMemoryUsage" << std::endl;
	emptyPullMemoryUsage();
	do {} while (0);
	//std::cout << "After emptyPullMemoryUsage" << std::endl;

	do {} while (0);
	//std::cout << std::endl << "Before parameterPullMemoryUsage" << std::endl;
	parameterPullMemoryUsage();
	do {} while (0);
	//std::cout << "After parameterPullMemoryUsage" << std::endl;

	do {} while (0);
	//std::cout << std::endl << "Before returnPullMemoryUsage" << std::endl;
	returnPullMemoryUsage();
	do {} while (0);
	//std::cout << "After returnPullMemoryUsage" << std::endl;
}

void StackfulCoroutinesMemoryUsage::emptyPullMemoryUsage()
{
	do {} while (0);
	//std::cout << "EmptyPullMemoryUsage: start" << std::endl;

	boost::coroutines2::coroutine<void>::pull_type pull_1(boost::bind(emptyPullFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "EmptyPullMemoryUsage: after creation pull_1" << std::endl;

	boost::coroutines2::coroutine<void>::pull_type pull_2(boost::bind(emptyPullFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "EmptyPullMemoryUsage: after creation pull_2" << std::endl;

	boost::coroutines2::coroutine<void>::pull_type pull_3(boost::bind(emptyPullFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "EmptyPullMemoryUsage: after creation pull_3" << std::endl;


	pull_1();
	do {} while (0);
	//std::cout << "EmptyPullMemoryUsage: after first call pull_1" << std::endl;

	pull_2();
	do {} while (0);
	//std::cout << "EmptyPullMemoryUsage: after first call pull_2" << std::endl;

	pull_3();
	do {} while (0);
	//std::cout << "EmptyPullMemoryUsage: after first call pull_3" << std::endl;

	do {} while (0);
	//std::cout << "EmptyPullMemoryUsage: end" << std::endl;
}

void StackfulCoroutinesMemoryUsage::emptyPullFunction(boost::coroutines2::coroutine<void>::push_type& yield)
{
	yield();
}

void StackfulCoroutinesMemoryUsage::parameterPullMemoryUsage()
{
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: start" << std::endl;

	boost::coroutines2::coroutine<void>::pull_type pull_1_int(boost::bind(intPullFunction, boost::placeholders::_1, 1));
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after creation pull_1_int" << std::endl;

	boost::coroutines2::coroutine<void>::pull_type pull_2_double(boost::bind(doublePullFunction, boost::placeholders::_1, 2.1));
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after creation pull_2_double" << std::endl;

	boost::coroutines2::coroutine<void>::pull_type pull_3_double(boost::bind(doublePullFunction, boost::placeholders::_1, 3.1));
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after creation pull_3_double" << std::endl;


	pull_1_int();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after first call pull_1" << std::endl;

	pull_2_double();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after first call pull_2" << std::endl;

	pull_3_double();
	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: after first call pull_3" << std::endl;

	do {} while (0);
	//std::cout << "ParameterPushMemoryUsage: end" << std::endl;
}

void StackfulCoroutinesMemoryUsage::intPullFunction(boost::coroutines2::coroutine<void>::push_type& yield, int intParameter)
{
	yield();
}

void StackfulCoroutinesMemoryUsage::doublePullFunction(boost::coroutines2::coroutine<void>::push_type& yield, double doubleParameter)
{
	yield();
}

void StackfulCoroutinesMemoryUsage::returnPullMemoryUsage() {
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: start" << std::endl;

	boost::coroutines2::coroutine<double>::pull_type pull_1(boost::bind(returnPullFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after creation pull_1 returned " << pull_1.get() << std::endl;

	boost::coroutines2::coroutine<double>::pull_type pull_2(boost::bind(returnPullFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after creation pull_2 returned " << pull_2.get() << std::endl;

	boost::coroutines2::coroutine<double>::pull_type pull_3(boost::bind(returnPullFunction, boost::placeholders::_1));
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after creation pull_3 returned " << pull_3.get() << std::endl;


	pull_1();
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after first call pull_1 returned " << pull_1.get() << std::endl;

	pull_2();
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after first call pull_2 returned " << pull_2.get() << std::endl;

	pull_3();
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after first call pull_3 returned " << pull_3.get() << std::endl;

	pull_1();
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after second call pull_1 returned " << pull_1.get() << std::endl;

	pull_2();
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after second call pull_2 returned " << pull_2.get() << std::endl;

	pull_3();
	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: after second call pull_3 returned " << pull_3.get() << std::endl;

	do {} while (0);
	//std::cout << "ReturnPushMemoryUsage: end" << std::endl;
}

void StackfulCoroutinesMemoryUsage::returnPullFunction(boost::coroutines2::coroutine<double>::push_type& yield) {
	do {} while (0);
	//std::cout << "ReturnPushFunction: first call, returning input 1.1" << std::endl;
	yield(1.1);
	do {} while (0);
	//std::cout << "ReturnPushFunction: second call, returning input 1.2" << std::endl;
	yield(1.2);
}