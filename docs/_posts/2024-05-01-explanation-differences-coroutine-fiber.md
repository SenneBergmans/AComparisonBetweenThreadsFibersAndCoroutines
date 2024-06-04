---
layout: post
title: Explanation - Difference between fibers and coroutines
date:   2024-05-01 10:00:00 +0200
categories: explanation fibers coroutines
permalink: "/explanation/fibers-coroutines-difference"
---
This page briefly summarises the theoretical differences between fibers and coroutines. Additionally, it provides a working implementation of the pseudocode examples provided in the paper.



Because fibers and coroutines are similar and are used interchangeably in some literature, their differences will be explained first.
Due to this paper's topic and focus, the two technologies are considered as two different technologies.
The following text highlights the differences between them to give the reader an idea of the strengths and weaknesses of the technologies so they can choose optimally between them.
The paper 'Distinguishing coroutines and fibers,' written by N. Goodspeed and O. Kowalke \[1\], was used as the primary source to get a clear idea of the differences between fibres and coroutines.
The examples given on this page are practical applications of the concepts defined in this paper.

Due to their similarity, it is possible to emulate the behaviour of fibers using coroutines and vice versa \[1, p. 3\].
Emulating a fiber with a coroutine is easy but might make the code more complex than just using fibers from the start.
The primary consideration when choosing between fibers and coroutines is the greater ease of use provided by fibers against the greater flexibility of coroutines.

Several C++ code examples will illustrate the differences in the inner workings of both technologies.
The Boost Fibers and Boost Coroutines2 libraries will implement fibers and coroutines [2], [3].


#### Usage
Fibers work similarly to threads. After creating a fiber, the programmer only needs to join it to ensure it is finished running before going out of scope.
Coroutines do not work in this way.
Coroutines resemble functions and need to be called by the programmer to be executed.
When using coroutines, the boolean operator of the coroutines is often used to check if the coroutine can still be called or is finished.
In some situations, it is not known beforehand how long a coroutine will run, but the join functionality of fibers can be recreated using coroutines.
Recreating the join functionality can be done in the following way: 
{% highlight C++ %}
#ifndef NOMINMAX //[4]
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>
#include <boost/coroutine2/all.hpp>

//The two test functions do the same thing. When called, they print the number that was provided and yield.
//When called again at a later time, they resume and print their number again and then complete.
void fiberFunction(int number) {
    std::cout << number << std::endl;
    boost::this_fiber::yield();
    std::cout << number << std::endl;
}

//The added pull_type parameter is used by the coroutine to yield.
//This parameter does not affect the inner workings of the function.
void coroutineFunction(boost::coroutines2::coroutine<void>::pull_type& yield, int number) {
    std::cout << number << std::endl;
    yield();
    std::cout << number << std::endl;
}

void fiberJoin() {
    boost::fibers::fiber fiber1(fiberFunction, 1);
    fiber1.join();
}

void coroutineJoin() {
    boost::coroutines2::coroutine<void>::push_type coroutine1(std::bind(&coroutineFunction, std::placeholders::_1, 1));
    while (coroutine1) {
        coroutine1();
    }
}

int main() {
    fiberJoin();
    coroutineJoin();
}
// --- Output ---  
// FiberJoin: 1 1
// CoroutineJoin : 1 1
// --- Output ---
{% endhighlight %}

This implementation works for a single coroutine. However, it will need to be extended if a fibre-like join needs to be implemented.
When using the while(coroutine) construction, the first coroutine will run entirely before the second coroutine is started. This order may not be the wanted behaviour.
A loop can be written that will go over all coroutines before continuing.
This way, the coroutine will work in the same way as fibers.
The loop shown in the AlternatingCoroutineJoin function can be extended to use a list of coroutines and go over all coroutines in the list if needed.
The two methods of finishing coroutines are worked out below.

{% highlight c++ %}
#ifndef NOMINMAX //[4]
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>
#include <boost/coroutine2/all.hpp>

void fiberFunction(int number) {
    std::cout << number << " ";
    boost::this_fiber::yield();
    std::cout << number << " ";
}

void coroutineFunction(boost::coroutines2::coroutine<void>::pull_type& yield, int number) {
    std::cout << number << " ";
    yield();
    std::cout << number << " ";
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
    boost::coroutines2::coroutine<void>::push_type coroutine(std::bind(&coroutineFunction, std::placeholders::_1, 1));
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

int main() {
    multiFiberJoin();
    oneByOneCoroutineJoin();
    alternatingCoroutineJoin();
}

// --- Output ---  
// MultiFiberJoin : 1 2 1 2
// OneByOneCoroutineJoin: 1 1 2 2
// AlternatingCoroutineJoin: 1 2 1 2
// --- Output ---
{% endhighlight %}

While the fiber example uses less code to ensure both fibers finish, it cannot easily recreate both coroutine examples using a single scheduler. 
It is possible to change the fiber scheduler to finish a single fiber before starting the other, but this requires more work than simply changing the way the functions are called, like in the coroutine example.

#### Scheduling
The second significant difference between fibers and coroutines is the use of a scheduler and the effect this has on the order of execution of the code.

{% highlight c++ %}
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

int main() {
    mainFiber();
    mainCoroutine();
}
// --- Output ---  
// MainFiber: main sub
// MainCoroutine : sub main
// --- Output ---
{% endhighlight %}

The example shows that the scheduler can call the main fiber to execute while the sub-function is active.
This call can happen because the fiber is not linked to its caller.
The only thing a function needs to do when it executes a fiber is to ensure the fiber is finished before destroying the fiber object.
It does not need to call the fiber specifically. It can be called outside of the main function.
Coroutines work differently than fibers because they work like functions. A coroutine can only be called when the caller has access to the coroutine handle.
The main function can pass the handle of coroutine1 to the sub-function, which will allow the sub-function to call coroutine1.
However, the sub-function cannot call coroutine1 if this is not done.

As stated previously, the differences between fibers and coroutines are relatively small.
When considering either technology, programmers should consider the easier usage of fibers due to their scheduler against the finer control of choosing where coroutines are called.

### References
[1] N. Goodspeed and O. Kowalke, "Distinguishing coroutines and fibers," ISO/IEC JTC1/SC22/WG21, Tech. Rep. N4024, May 2014. [Online]. Available: [https://open-std.org/JTC1/SC22/WG21/docs/papers/2014/n4024.pdf](https://open-std.org/JTC1/SC22/WG21/docs/papers/2014/n4024.pdf)

[2] O. Kowalke, "Boost C++ Libraries Fiber," boost.org, Accessed: May
13, 2024. [Online]. Available: [https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/index.html](https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/index.html)

[3] O. Kowalke, "Boost C++ Libraries Coroutine2," boost.org, Accessed: May
21, 2024. [Online]. Available: [https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/index.html](https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/index.html)

[4] Zitrax and H. Passant. "Answer to warning C4003: not enough actual parameters for macro 'max' - Visual Studio 2010 C++." stackoverflow.com. Accessed: Mar. 1, 2024. [Online.] Available: [https://stackoverflow.com/a/6884251](https://stackoverflow.com/a/6884251)