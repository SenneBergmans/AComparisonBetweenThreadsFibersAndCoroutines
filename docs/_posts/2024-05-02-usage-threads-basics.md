---
layout: post
title:  "Usage - Threads - Basics"
date:   2024-05-02 10:00:00 +0200
categories: usage threads basics
permalink: "/usage/threads/basics"
---
This page goes over the absolute basics of Boost threads [1].
It goes over topics like creating a thread and joining it.
Readers familiar with threads might find this page too simplistic.
Therefore, they should start with the producer-consumer page because the complete example might be a better starting point for them.

The page will start with simple examples showing how threads are created and joined and will look at the way thread scheduling affects the order of execution.
A custom class containing all the example functions will be used.
This usage of a class is done because the creation of threads works differently for static and non-static thread functions.
When the function the thread should run is not part of any class, the same code as the example for static functions can be used.
The class is defined as follows in the header file: 
{% highlight c++ %}
#include <string>
class ThreadsBasics
{
public:
    static void staticExample();
    void memberExample();
    static void parameterExample();

private:
    static void staticFunction();
    void memberFunction();
    static void parameterFunction(std::string firstString, std::string secondString);
};
{% endhighlight %}

When a thread is created, it will have the type boost::thread.
A thread can be created using boost::thread(functionName), creating a thread that executes the functionName.
This thread can be saved inside of a variable with the type boost::thread.
The thread can be saved using the assignment operator or a shorthand notation.
The external scheduler used by threads will handle calling the thread.
The only action the programmer needs to take is to join the thread.
Joining a thread will pause the current function until the thread has finished running.
The following code example creates a thread and joins it to ensure it finishes running.
Please note that threads do not work deterministically, so the provided output can differ based on the order in which the threads are executed.
{% highlight c++ %}
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

// --- Output ---
// staticExample: started running
// staticExample: started creating a new thread
// staticExample: finished creating a new thread
// staticExample: started waiting for myThread to join
// staticFunction: started running
// staticFunction: finished running
// staticExample: finished waiting for myThread to join
// staticExample: finished running
// --- Output ---
{% endhighlight %}

When using non-static functions in a class, the class instance of the function also needs to be provided.
This instance can be provided using the second parameter of the thread constructor.
If the function creating the thread belongs to the same class instance as the thread, then the 'this' keyword can reference the class instance. 
{% highlight c++ %}
#include "ThreadsBasics.h"
#include <iostream>
#include <boost/thread.hpp>

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

// --- Output ---
// memberExample: started running
// memberExample: started creating a new thread
// memberExample: finished creating a new thread
// memberExample: started waiting for myThread to join
// memberFunction: started running
// memberFunction: finished running
// memberExample: finished waiting for myThread to join
// memberExample: finished running
// --- Output ---
{% endhighlight %}

Any parameters the function the thread will run should require can be added after the function name and optional instance parameters.
{% highlight c++ %}
#include "ThreadsBasics.h"
#include <iostream>
#include <boost/thread.hpp>

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

// --- Output ---
// parameterExample: started running
// parameterExample: started creating a new thread
// parameterExample: finished creating a new thread
// parameterExample: started waiting for myThread to join
// parameterFunction: started running
// parameterFunction: the first string is String 1
// parameterFunction: the second string is String 2
// parameterFunction: finished running
// parameterExample: finished waiting for myThread to join
// parameterExample: finished running
// --- Output ---
{% endhighlight %}

### References
[1] A. Williams and V. J. Botet Escriba, "Boost C++ Libraries Thread
4.8.0," boost.org, Accessed: Mar. 5, 2024. [Online]. Available:
[https://www.boost.org/doc/libs/1_85_0/doc/html/thread.html](https://www.boost.org/doc/libs/1_85_0/doc/html/thread.html)