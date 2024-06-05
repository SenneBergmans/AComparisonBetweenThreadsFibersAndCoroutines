#include "ThreadsBasics.h"

#include <iostream>
#include <boost/thread.hpp>

void ThreadsBasics::staticExample() {
    std::cout << "staticExample: started running" << std::endl;

    std::cout << "staticExample: started creating a new thread" << std::endl;
    boost::thread myThread = boost::thread(staticFunction);
    //boost::thread myThread(staticFunction);
    std::cout << "staticExample: finished creating a new thread" << std::endl;

    std::cout << "staticExample: started waiting for myThread to join" << std::endl;
    myThread.join();
    std::cout << "staticExample: finished waiting for myThread to join" << std::endl;

    std::cout << "staticExample: finished running" << std::endl;
}

void ThreadsBasics::staticFunction() {
    std::cout << "staticFunction: started running" << std::endl;
    //Do something
    std::cout << "staticFunction: finished running" << std::endl;
}

void ThreadsBasics::memberExample() {
	std::cout << "memberExample: started running" << std::endl;

	std::cout << "memberExample: started creating a new thread" << std::endl;
	boost::thread myThread = boost::thread(&ThreadsBasics::memberFunction, this);
	//ThreadsBasics instance = ThreadsBasics();
	//boost::thread myThread = boost::thread(&ThreadsBasics::memberFunction, &instance);
	std::cout << "memberExample: finished creating a new thread" << std::endl;

	std::cout << "memberExample: started waiting for myThread to join" << std::endl;
	myThread.join();
	std::cout << "memberExample: finished waiting for myThread to join" << std::endl;

	std::cout << "memberExample: finished running" << std::endl;
}

void ThreadsBasics::memberFunction() {
	std::cout << "memberFunction: started running" << std::endl;
	//Do something
	std::cout << "memberFunction: finished running" << std::endl;
}

void ThreadsBasics::parameterExample() {
	std::cout << "parameterExample: started running" << std::endl;

	std::cout << "parameterExample: started creating a new thread" << std::endl;
	boost::thread parameterThread(parameterFunction, "String 1", "String 2");
	std::cout << "parameterExample: finished creating a new thread" << std::endl;

	std::cout << "parameterExample: started waiting for parameterThread to join " << std::endl;
	parameterThread.join();
	std::cout << "parameterExample: finished waiting for parameterThread to join" << std::endl;

	std::cout << "parameterExample: finished running" << std::endl;
}

void ThreadsBasics::parameterFunction(std::string firstString, std::string secondString) {
	std::cout << "parameterFunction: started running" << std::endl;
	std::cout << "parameterFunction: the first string is " << firstString << std::endl;
	std::cout << "parameterFunction: the second string is " << secondString << std::endl;
	std::cout << "parameterFunction: finished running" << std::endl;
}