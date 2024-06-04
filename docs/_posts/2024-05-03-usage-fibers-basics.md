---
layout: post
title:  "Usage - Fibers - Basics"
date:   2024-05-03 10:00:00 +0200
categories:  usage fibers basics
permalink: "/usage/fibers/basics"
---
This page goes over the absolute basics of Boost fibers [1].
It covers creating and joining fibers.
Readers familiar with fibers might find starting with the producer-consumer page covering fibers more useful.

The page will start with simple examples showing how fibers are created and joined and will look at the way fiber scheduling affects the order of execution.
Additional code is used when including the fiber library due to an issue when using the Boost library. An explanation and the solution can be found in [2]. 
A custom class containing all the example functions will be used.
This usage of a class is done because the creation of fibers works differently for static and non-static thread functions.
When the function the fiber should run is not part of any class, the same code as the example for static functions can be used.
The class is defined as follows in the header file:
{% highlight c++ %}
class FibersBasics
{
public:
    static void staticExample();
    void memberExample();
    static void parameterExample();
    static void noYieldExample();
    static void yieldExample();

private:
    static void staticFunction();
    void memberFunction();
    static void parameterFunction();
    static void noYieldFunction();
    static void yieldFunction();
}
{% endhighlight %}
When a fiber is created, it will have the type boost::fibers::fiber.
A fiber can be created using boost:fibers::fiber(functionName), creating a fiber that executes the function named functionName.
This fiber can be stored in a variable with the type boost::fibers::fiber.
Fibers can also be saved using the assignment operator or a shorthand notation.
The scheduler used by threads will handle calling the fiber.
The programmer does need to join the fiber.
Joining the fiber ensures it will finish before going out of scope by pausing the main function until this has happened.
The following code example creates a fiber and joins it to ensure it finishes running.
{% highlight c++ %}
#include "FibersBasics.h"
#include <iostream>
#ifndef NOMINMAX //[2]
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>

void FibersBasics::staticExample() {
    std::cout << "staticExample: started running" << std::endl;

    std::cout << "staticExample: started creating a new fiber" << std::endl;
    boost::fibers::fiber myFiber = boost::fibers::fiber(staticFunction);
    //boost::fibers::fiber myFiber(exampleFunction);
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

// --- Output ---
// staticExample: started running
// staticExample: started creating a new fiber
// staticExample: finished creating a new fiber
// staticExample: started waiting for myFiber to join
// staticFunction: started running
// staticFunction: finished running
// staticExample: finished waiting for myFiber to join
// staticExample: finished running
// --- Output ---
{% endhighlight %}
When using non-static functions in a class, the class instance of the function also needs to be provided.
This instance can be provided using the second parameter of the fiber constructor.
If the function creating the fiber belongs to the same class instance as the fiber, then the 'this' keyword can reference the class instance.
{% highlight c++ %}
#include "FibersBasics.h"
#include <iostream>
#ifndef NOMINMAX //[2]
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>

void FibersBasics::memberExample() {
	std::cout << "memberExample: started running" << std::endl;

    std::cout << "memberExample: started creating a new fiber" << std::endl;
    boost::fibers::fiber myFiber = boost::fibers::fiber(&FibersBasicExample::memberFunction, this);
    //FibersBasicExample instance = FibersBasicExample();
    //boost::fibers::fiber myFiber(&FibersBasicExample::memberFunction, &instance);
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

// --- Output ---
// memberExample: started running
// memberExample: started creating a new fiber
// memberExample: finished creating a new fiber
// memberExample: started waiting for myFiber to join
// memberFunction: started running
// memberFunction: finished running
// memberExample: finished waiting for myFiber to join
// memberExample: finished running
// --- Output ---
{% endhighlight %}
Any required parameters can be added after the function name and optional instance parameters.
{% highlight c++ %}
#include "FibersBasics.h"
#include <iostream>
#ifndef NOMINMAX //[2]
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>

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

// --- Output ---
// parameterExample: started running
// parameterExample: started creating a new fiber
// parameterExample: finished creating a new fiber
// parameterExample: started waiting for parameterFiber to join
// parameterFunction: started running
// parameterFunction: the first string is String 1
// parameterFunction: the second string is String 2
// parameterFunction: finished running
// parameterExample: finished waiting for parameterFiber to join
// parameterExample: finished running
// --- Output ---
{% endhighlight %}
Unlike threads, which use preemptive scheduling, fibers use cooperative scheduling.
This use of cooperative scheduling means that fibers must define when they yield, which is not required when using threads.
This yield is done using the boost::this_fiber::yield() function.
Adding this function in the function the fiber will run will make the fiber yield when it executes that line of code.
When the fiber is called again, it starts right after the line where it was yielded.
The following example tries to illustrate this behaviour.
Three fibers will be created, and each will print its number twice.
A version with and without the yield statement will illustrate the difference. The version that uses yield has been edited explicitly, so the first fiber does not use the yield function because this illustrates the usage of the scheduler better than when all functions yield.
{% highlight c++ %}
#include "FibersBasics.h"
#include <iostream>
#ifndef NOMINMAX //[2]
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>

void FibersBasics::noYieldExample() {
    std::cout << "noYieldExample: started running" << std::endl;

    std::cout << "noYieldExample: started creating three fibers" << std::endl;
    boost::fibers::fiber fiber1 = boost::fibers::fiber(noYieldFunction);
    boost::fibers::fiber fiber2 = boost::fibers::fiber(noYieldFunction);
    boost::fibers::fiber fiber3 = boost::fibers::fiber(noYieldFunction);
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

// --- Output ---
// noYieldExample: started running
// noYieldExample: started creating three fibers
// noYieldExample: finished creating three fibers
// noYieldExample: started waiting for fiber1 to join
// noYieldFunction: 1
// noYieldFunction: 1
// noYieldFunction: 2
// noYieldFunction: 2
// noYieldFunction: 3
// noYieldFunction: 3
// noYieldExample: finished waiting for fiber1 to join
// noYieldExample: started waiting for fiber2 to join
// noYieldExample: finished waiting for fiber2 to join
// noYieldExample: started waiting for fiber3 to join
// noYieldExample: finished waiting for fiber3 to join
// noYieldExample: finished running
//
// yieldExample: started running
// yieldExample: started creating three fibers
// yieldExample: finished creating three fibers
// yieldExample: started waiting for fiber1 to join
// yieldFunction: 1
// yieldFunction: 2
// yieldFunction: 3
// yieldFunction: 1
// yieldFunction: 2
// yieldFunction: 3
// yieldExample: finished waiting for fiber1 to join
// yieldExample: started waiting for fiber2 to join
// yieldExample: finished waiting for fiber2 to join
// yieldExample: started waiting for fiber3 to join
// yieldExample: finished waiting for fiber3 to join
// yieldExample: finished running
{% endhighlight %}
The default scheduler of Boost fibers uses the round-robin method to pick its next fiber [3].
When choosing fibers, a first in, first out queue is used.
The main function can also be seen as part of this queue.
In the noYield example, fiber1.join() will be called, causing the main function to stop and go to the back of the queue. Because all fibers finish in a single call, they all print their number twice, after which the main function takes over again and goes over the subsequent joins, which do nothing anymore because the fibers they join have already been completed.
In the yield example, the main fiber will wait for fiber1 to join and stop.
Fiber 1 takes over and completes in a single call, after which fiber2 and fiber3 are next in the queue. After all three fibers have been called, the main function will resume because fiber1 has finished. The main function then joins fiber2, and fiber2 and fiber3 are completed, after which the main function finishes.
The yield example shows that the order in which fibers are created matters because this is the order in which they are inserted into the queue.
Although unimportant in this small example, this creation order can impact the performance of a program, as will be explained in the context of the producer-consumer example on the next page.

### References
[1] O. Kowalke. "Boost C++ Libraries Fiber." boost.org. Accessed: May 13, 2024. [Online.] Available: [https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/index.html](https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/index.html)

[2] Zitrax and H. Passant. "Answer to warning C4003: not enough actual parameters for macro 'max' - Visual Studio 2010 C++." stackoverflow.com. Accessed: Mar. 1, 2024. [Online.] Available: [https://stackoverflow.com/a/6884251](https://stackoverflow.com/a/6884251)

[3] O. Kowalke. "Boost C++ Libraries Fiber Scheduling." boost.org. Accessed: May 13, 2024. [Online.] Available: [https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/fiber/scheduling.html](https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/fiber/scheduling.html)