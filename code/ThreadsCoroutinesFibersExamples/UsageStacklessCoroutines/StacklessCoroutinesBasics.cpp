#include "StacklessCoroutinesBasics.h"

#include <iostream>

void StacklessCoroutinesBasics::staticExample() {
    std::cout << "staticExample: started running" << std::endl;

    std::cout << "staticExample: started creating a new coroutine" << std::endl;
    CoroutineHandler<bool> myCoroutine = staticFunction();
    std::cout << "staticExample: finished creating a new coroutine" << std::endl;

    std::cout << "staticExample: started first coroutine call" << std::endl;
    myCoroutine();
    std::cout << "staticExample: finished first coroutine call" << std::endl;

    std::cout << "staticExample: started second coroutine call" << std::endl;
    myCoroutine();
    std::cout << "staticExample: finished second coroutine call" << std::endl;

    std::cout << "staticExample: finished running" << std::endl;
}

CoroutineHandler<bool> StacklessCoroutinesBasics::staticFunction()
{
    std::cout << "staticFunction: started running" << std::endl;

    std::cout << "staticFunction: start yield" << std::endl;
    co_yield true;
    std::cout << "staticFunction: finished yield" << std::endl;
    
    std::cout << "staticFunction: finished running" << std::endl;
}

void StacklessCoroutinesBasics::memberExample() {
    std::cout << "memberExample: started running" << std::endl;

    std::cout << "memberExample: started creating a new coroutine" << std::endl;
    //CoroutineHandler<bool> myCoroutine = memberFunction();
    StacklessCoroutinesBasics instance = StacklessCoroutinesBasics();
    CoroutineHandler<bool> myCoroutine = instance.memberFunction();
    std::cout << "memberExample: finished creating a new coroutine" << std::endl;

    std::cout << "memberExample: started first coroutine call" << std::endl;
    myCoroutine();
    std::cout << "memberExample: finished first coroutine call" << std::endl;

    std::cout << "memberExample: finished running" << std::endl;
}

CoroutineHandler<bool> StacklessCoroutinesBasics::memberFunction()
{
    std::cout << "memberFunction: started running" << std::endl;
    //Do something
    std::cout << "memberFunction: finished running" << std::endl;
    co_return;
}

void StacklessCoroutinesBasics::parameterExample() {
    std::cout << "parameterExample: started running" << std::endl;

    std::cout << "parameterExample: started creating a new coroutine" << std::endl;
    CoroutineHandler<bool> myCoroutine = parameterFunction("String 1", "String 2");
    std::cout << "parameterExample: finished creating a new coroutine" << std::endl;

    std::cout << "parameterExample: started first coroutine call" << std::endl;
    myCoroutine();
    std::cout << "parameterExample: finished first coroutine call" << std::endl;

    std::cout << "parameterExample: finished running" << std::endl;
}

CoroutineHandler<bool> StacklessCoroutinesBasics::parameterFunction(std::string firstString, std::string secondString) {
    std::cout << "parameterFunction: started running" << std::endl;
    std::cout << "parameterFunction: the first string is " << firstString << std::endl;
    std::cout << "parameterFunction: the second string is " << secondString << std::endl;
    std::cout << "parameterFunction: finished running" << std::endl;
    co_return;
}