#include "FibersBasics.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <boost/fiber/all.hpp>

void FibersBasics::staticExample() {
    std::cout << "staticExample: started running" << std::endl;

    std::cout << "staticExample: started creating a new fiber" << std::endl;
    boost::fibers::fiber myFiber = boost::fibers::fiber(staticFunction);
    //boost::fibers::fiber myFiber(staticFunction);
    std::cout << "staticExample: finished creating a new fiber" << std::endl;

    std::cout << "staticExample: started waiting for myFiber to join" << std::endl;
    myFiber.join();
    std::cout << "staticExample: finished waiting for myFiber to join" << std::endl;

    std::cout << "staticExample: finished running" << std::endl;
}

void FibersBasics::staticFunction() {
    std::cout << "staticFunction: started running" << std::endl;
    //Do something
    std::cout << "staticFunction: finished running" << std::endl;
}

void FibersBasics::memberExample() {
    std::cout << "memberExample: started running" << std::endl;

    std::cout << "memberExample: started creating a new fiber" << std::endl;
    boost::fibers::fiber myFiber = boost::fibers::fiber(&FibersBasics::memberFunction, this);
    //FibersBasics instance = FibersBasics();
    //boost::fibers::fiber myFiber(&FibersBasics::memberFunction, &instance);
    std::cout << "memberExample: finished creating a new fiber" << std::endl;

    std::cout << "memberExample: started waiting for myFiber to join" << std::endl;
    myFiber.join();
    std::cout << "memberExample: finished waiting for myFiber to join" << std::endl;

    std::cout << "memberExample: finished running" << std::endl;
}

void FibersBasics::memberFunction() {
    std::cout << "memberFunction: started running" << std::endl;
    //Do something
    std::cout << "memberFunction: finished running" << std::endl;
}

void FibersBasics::parameterExample() {
    std::cout << "parameterExample: started running" << std::endl;

    std::cout << "parameterExample: started creating a new fiber" << std::endl;
    boost::fibers::fiber parameterFiber(parameterFunction, "String 1", "String 2");
    std::cout << "parameterExample: finished creating a new fiber" << std::endl;

    std::cout << "parameterExample: started waiting for parameterFiber to join" << std::endl;
    parameterFiber.join();
    std::cout << "parameterExample: finished waiting for parameterFiber to join" << std::endl;

    std::cout << "parameterExample: finished running" << std::endl;
}

void FibersBasics::parameterFunction(std::string firstString, std::string secondString) {
    std::cout << "parameterFunction: started running" << std::endl;
    std::cout << "parameterFunction: the first string is " << firstString << std::endl;
    std::cout << "parameterFunction: the second string is " << secondString << std::endl;
    std::cout << "parameterFunction: finished running" << std::endl;
}

void FibersBasics::noYieldExample() {
    std::cout << "noYieldExample: started running" << std::endl;

    std::cout << "noYieldExample: started creating three fibers" << std::endl;
    boost::fibers::fiber fiber1 = boost::fibers::fiber(noYieldFunction, 1);
    boost::fibers::fiber fiber2 = boost::fibers::fiber(noYieldFunction, 2);
    boost::fibers::fiber fiber3 = boost::fibers::fiber(noYieldFunction, 3);
    std::cout << "noYieldExample: finished creating three fibers" << std::endl;

    std::cout << "noYieldExample: started waiting for fiber1 to join" << std::endl;
    fiber1.join();
    std::cout << "noYieldExample: finished waiting for fiber1 to join" << std::endl;
    std::cout << "noYieldExample: started waiting for fiber2 to join" << std::endl;
    fiber2.join();
    std::cout << "noYieldExample: finished waiting for fiber2 to join" << std::endl;
    std::cout << "noYieldExample: started waiting for fiber3 to join" << std::endl;
    fiber3.join();
    std::cout << "noYieldExample: finished waiting for fiber3 to join" << std::endl;

    std::cout << "noYieldExample: finished running" << std::endl;
}

void FibersBasics::noYieldFunction(int number) {
    std::cout << "noYieldFunction: " << std::to_string(number) << std::endl;

    std::cout << "noYieldFunction: " << std::to_string(number) << std::endl;
}

void FibersBasics::yieldExample() {
    std::cout << "yieldExample: started running" << std::endl;

    std::cout << "yieldExample: started creating three fibers" << std::endl;
    boost::fibers::fiber fiber1 = boost::fibers::fiber(noYieldFunction, 1);
    boost::fibers::fiber fiber2 = boost::fibers::fiber(yieldFunction, 2);
    boost::fibers::fiber fiber3 = boost::fibers::fiber(yieldFunction, 3);
    std::cout << "yieldExample: finished creating three fibers" << std::endl;

    std::cout << "yieldExample: started waiting for fiber1 to join" << std::endl;
    fiber1.join();
    std::cout << "yieldExample: finished waiting for fiber1 to join" << std::endl;
    std::cout << "yieldExample: started waiting for fiber2 to join" << std::endl;
    fiber2.join();
    std::cout << "yieldExample: finished waiting for fiber2 to join" << std::endl;
    std::cout << "yieldExample: started waiting for fiber3 to join" << std::endl;
    fiber3.join();
    std::cout << "yieldExample: finished waiting for fiber3 to join" << std::endl;

    std::cout << "yieldExample: finished running" << std::endl;
}

void FibersBasics::yieldFunction(int number) {
    std::cout << "yieldFunction: " << std::to_string(number) << std::endl;
    boost::this_fiber::yield();
    std::cout << "yieldFunction: " << std::to_string(number) << std::endl;
}