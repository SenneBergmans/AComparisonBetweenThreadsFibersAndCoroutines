#include "StacklessCoroutinesMemoryUsage.h"

void StacklessCoroutinesMemoryUsage::memoryUsage()
{
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

	do {} while (0);
	//std::cout << std::endl << "Before returnTSMemoryUsage" << std::endl;
	returnMemoryUsage();
	do {} while (0);
	//std::cout << "After returnTSMemoryUsage" << std::endl;
}

void StacklessCoroutinesMemoryUsage::emptyMemoryUsage()
{
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: start" << std::endl;

	CoroutineHandler<bool> push_1 = emptyFunction();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after creation push_1" << std::endl;

	CoroutineHandler<bool> push_2 = emptyFunction();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after creation push_2" << std::endl;

	CoroutineHandler<bool> push_3 = emptyFunction();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after creation push_3" << std::endl;

	push_1();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after first call push_1" << std::endl;

	push_2();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after first call push_2" << std::endl;

	push_3();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after first call push_3" << std::endl;

	push_1();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after second call push_1" << std::endl;

	push_2();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after second call push_2" << std::endl;

	push_3();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after second call push_3" << std::endl;

	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: end" << std::endl;
}

CoroutineHandler<bool> StacklessCoroutinesMemoryUsage::emptyFunction()
{
	co_yield true;
	co_return;
}

void StacklessCoroutinesMemoryUsage::parameterMemoryUsage()
{
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: start" << std::endl;

	CoroutineHandler<bool> oneDouble = oneDoubleFunction(1);
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after creation oneDouble" << std::endl;

	CoroutineHandler<bool> twoDoubles = twoDoublesFunction(1, 2);
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after creation twoDoubles" << std::endl;

	CoroutineHandler<bool> threeDoubles = threeDoublesFunction(1, 2, 3);
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after creation threeDoubles" << std::endl;

	oneDouble();
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after first call oneDouble" << std::endl;

	twoDoubles();
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after first call twoDoubles" << std::endl;

	threeDoubles();
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after first call threeDoubles" << std::endl;

	oneDouble();
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after second call oneDouble" << std::endl;

	twoDoubles();
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after second call twoDoubles" << std::endl;

	threeDoubles();
	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: after second call threeDoubles" << std::endl;

	do {} while (0);
	//std::cout << "ParameterTSMemoryUsage: end" << std::endl;
}

CoroutineHandler<bool> StacklessCoroutinesMemoryUsage::oneDoubleFunction(double one)
{
	co_yield true;
	co_return;
}

CoroutineHandler<bool> StacklessCoroutinesMemoryUsage::twoDoublesFunction(double one, double two)
{
	co_yield true;
	co_return;
}

CoroutineHandler<bool> StacklessCoroutinesMemoryUsage::threeDoublesFunction(double one, double two, double three)
{
	co_yield true;
	co_return;
}

void StacklessCoroutinesMemoryUsage::returnMemoryUsage() {
	do {} while (0);
	//std::cout << "ReturnTSMemoryUsage: start" << std::endl;

	CoroutineHandler<double> push_1 = returnFunction();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after creation push_1" << std::endl;

	CoroutineHandler<double> push_2 = returnFunction();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after creation push_2" << std::endl;

	CoroutineHandler<double> push_3 = returnFunction();
	do {} while (0);
	//std::cout << "EmptyTSMemoryUsage: after creation push_3" << std::endl;


	double result = push_1();
	do {} while (0);
	//std::cout << "ReturnTSMemoryUsage: after first call push_1 returned " << result << std::endl;

	result = push_2();
	do {} while (0);
	//std::cout << "ReturnTSMemoryUsage: after first call push_2 returned " << result << std::endl;

	result = push_3();
	do {} while (0);
	//std::cout << "ReturnTSMemoryUsage: after first call push_3 returned " << result << std::endl;

	result = push_1();
	do {} while (0);
	//std::cout << "ReturnTSMemoryUsage: after second call push_1 returned " << result << std::endl;

	result = push_2();
	do {} while (0);
	//std::cout << "ReturnTSMemoryUsage: after second call push_2 returned " << result << std::endl;

	result = push_3();
	do {} while (0);
	//std::cout << "ReturnTSMemoryUsage: after second call push_3 returned " << result << std::endl;

	do {} while (0);
	//std::cout << "ReturnTSMemoryUsage: end" << std::endl;
}

CoroutineHandler<double> StacklessCoroutinesMemoryUsage::returnFunction() {
	double firstCallReturn = 1.1;
	do {} while (0);
	//std::cout << "ReturnTSFunction: first call, returning input 1.1" << std::endl;
	co_yield firstCallReturn;

	double secondCallReturn = 1.2;
	do {} while (0);
	//std::cout << "ReturnTSFunction: second call, returning input 1.2" << std::endl;
	co_yield secondCallReturn;

	double finalCallReturn = 1.3;
	co_return;
}