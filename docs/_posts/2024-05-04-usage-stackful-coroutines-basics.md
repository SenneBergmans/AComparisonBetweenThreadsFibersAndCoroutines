---
layout: post
title:  "Usage - Stackful Coroutines - Basics"
date:   2024-05-04 10:00:00 +0200
categories: usage coroutines stackfull_coroutines basics
permalink: "/usage/coroutines/stackful/basics"
---
This page covers the basic usage of Boost coroutines, specifically the Boost coroutine2 library.
It covers the creation and calling of stackful coroutines.
Readers familiar with coroutines and the differences between stackful and stackless coroutines might find starting with the producer-consumer page covering stackful coroutines more useful.

To avoid confusion for the remainder of this page, any mention of coroutines without the word stackful will also refer to stackful coroutines.
Some simple examples will show how Boost coroutines are created and called, and the differences between push-type and pull-type coroutines will be examined. 
A custom class containing all the example functions will be used.
This class is used because the creation of coroutines works differently for static and non-static thread functions.
When the function the coroutine should run is not part of any class, the same code as the example for static functions can be used.
The class is defined as follows in the header file: 
{% highlight c++ %}

#include <iostream>
#include <boost/coroutine2/all.hpp>

class StackfulCoroutinesBasics
{
public:
    static void staticPushExample();
    static void staticPullExample();
    void memberExample();
    static void parameterExample();
    static void nestedYieldExample();

private:
    static void staticPushFunction(boost::coroutines2::coroutine<int>::pull_type& yield);
    static void staticPullFunction(boost::coroutines2::coroutine<bool>::push_type& yield);
    void memberFunction(boost::coroutines2::coroutine<void>::pull_type& yield);
    static void parameterFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::string firstString, std::string secondString);
    static void mainYieldFunction(boost::coroutines2::coroutine<void>::pull_type& yield);
    static void subYieldFunction(boost::coroutines2::coroutine<void>::pull_type& yield);
};
{% endhighlight %}

Two basic examples will create a push-type and pull-type coroutine and explain their differences.
Aside from the differences explained in the following example, the method for creating push-type and pull-type coroutines is the same. Thus, the other examples will only be worked out using push-type coroutines.
When a coroutine is created, it will have the type boost::coroutines2::coroutine<parameterType>::push_type or boost::coroutines2::coroutine<parameterType>::pull_type.
A push-type coroutine can require a parameter of parameterType on each call. It can thus be used to pass parameters to the coroutine while running.
If the parameterType is void, no parameter must be provided to the coroutine.
Because a push-type coroutine can require a parameter, it is first called when the programmer calls the coroutine. 
A pull-type coroutine will return a parameter of parameterType.
This return value can retrieve data from the coroutine on each call.
The pull-type coroutine's most recent return value can be obtained using the get function. To avoid a situation where this get function is called before the coroutine has been called and generated a return value, the pull-type coroutine runs for the first time immediately after creation.
It is essential to know this difference because a push-type coroutine with the same yields as a pull-type coroutine needs to be called a single time more by the programmer.
The following example creates both a push- and pull-type coroutine and tries to illustrate the difference.

{% highlight c++ %}
void StackfulCoroutinesBasics::staticPushExample() {
    std::cout << "staticPushExample: started running" << std::endl;

    std::cout << "staticPushExample: started creating a new push-type coroutine" << std::endl;
    boost::coroutines2::coroutine<int>::push_type myPushCoroutine = boost::coroutines2::coroutine<int>::push_type(std::bind(staticPushFunction, std::placeholders::_1));
    std::cout << "staticPushExample: finished creating a new push-type coroutine" << std::endl;

    std::cout << "staticPushExample: started first push-type coroutine call" << std::endl;
    myPushCoroutine(1);
    std::cout << "staticPushExample: finished first push-type coroutine call" << std::endl;

    std::cout << "staticPushExample: started second push-type coroutine call" << std::endl;
    myPushCoroutine(2);
    std::cout << "staticPushExample: finished second push-type coroutine call" << std::endl;

    std::cout << "staticPushExample: finished running" << std::endl;
}

void StackfulCoroutinesBasics::staticPushFunction(boost::coroutines2::coroutine<int>::pull_type& yield)
{
    std::cout << "staticPushFunction: started running" << std::endl;
    
    std::cout << "staticPushFunction: the first call received value " << std::to_string(yield.get()) << std::endl;
    std::cout << "staticPushFunction: start yield" << std::endl;
    yield();
    std::cout << "staticPushFunction: finished yield" << std::endl;
    std::cout << "staticPushFunction: the second call received value " << std::to_string(yield.get()) << std::endl;

    std::cout << "staticPushFunction: finished running" << std::endl;
}

// --- Output ---
// staticPushExample: started running
// staticPushExample: started creating a new push-type coroutine
// staticPushExample: finished creating a new push-type coroutine
// staticPushExample: started first push-type coroutine call
// staticPushFunction: started running
// staticPushFunction: the first call received value 1
// staticPushFunction: start yield
// staticPushExample: finished first push-type coroutine call
// staticPushExample: started second push-type coroutine call
// staticPushFunction: finished yield
// staticPushFunction: the second call received value 2
// staticPushFunction: finished running
// staticPushExample: finished second push-type coroutine call
// staticPushExample: finished running
// --- Output ---
{% endhighlight %}

{% highlight c++ %}
void StackfulCoroutinesBasics::staticPullExample() {
    std::cout << "staticPullExample: started running" << std::endl;

    std::cout << "staticPullExample: started creating a new pull-type coroutine" << std::endl;
    boost::coroutines2::coroutine<bool>::pull_type myPullCoroutine(std::bind(staticPullFunction, std::placeholders::_1));
    std::cout << "staticPullExample: finished creating a new pull-type coroutine" << std::endl;

    std::cout << "staticPullExample: the return value of myPullCoroutine is " << myPullCoroutine.get() << std::endl;

    std::cout << "staticPullExample: started first (manual) pull-type coroutine call" << std::endl;
    myPullCoroutine();
    std::cout << "staticPullExample: finished first (manual) pull-type coroutine call" << std::endl;

    std::cout << "staticPullExample: the return value of myPullCoroutine is " << myPullCoroutine.get() << std::endl;

    std::cout << "staticPullExample: started second (manual) pull-type coroutine call" << std::endl;
    myPullCoroutine();
    std::cout << "staticPullExample: finished second (manual) pull-type coroutine call" << std::endl;

    std::cout << "staticPullExample: the return value when the function ends remains the same as the value from the last yield and is " << myPullCoroutine.get() << std::endl;

    std::cout << "staticPullExample: finished running" << std::endl;
}

void StackfulCoroutinesBasics::staticPullFunction(boost::coroutines2::coroutine<bool>::push_type& yield)
{
    std::cout << "staticPullFunction: started running" << std::endl;

    std::cout << "staticPullFunction: start first yield" << std::endl;
    yield(true);
    std::cout << "staticPullFunction: finished first yield" << std::endl;

    std::cout << "staticPullFunction: start second yield" << std::endl;
    yield(false);
    std::cout << "staticPullFunction: finished second yield" << std::endl;

    std::cout << "staticPullFunction: finished running" << std::endl;
}

// --- Output ---
// staticPullExample: started running
// staticPullExample: started creating a new pull-type coroutine
// staticPullFunction: started running
// staticPullFunction: start first yield
// staticPullExample: finished creating a new pull-type coroutine
// staticPullExample: the return value of myPullCoroutine is 1
// staticPullExample: started first (manual) pull-type coroutine call
// staticPullFunction: finished first yield
// staticPullFunction: start second yield
// staticPullExample: finished first (manual) pull-type coroutine call
// staticPullExample: the return value of myPullCoroutine is 0
// staticPullExample: started second (manual) pull-type coroutine call
// staticPullFunction: finished second yield
// staticPullFunction: finished running
// staticPullExample: finished second (manual) pull-type coroutine call
// staticPullExample: the return value when the function ends remains the same as the value from the last yield and is 0
// staticPullExample: finished running
// --- Output ---
{% endhighlight %}

Comparing the basic example created using coroutines with the basic example of fibers shows three main differences.

First, the coroutine implementation requires an additional parameter in the functions.
The type of this parameter is a matching coroutine of the opposite type.
A push coroutine expecting integers as input (boost::coroutine2::coroutine\<int\>::push_type) requires a pull-type parameter that returns integers (boost::coroutine2::coroutine\<int\>::pull_type) in the function it will run.
Pull-type coroutines require a push-type parameter.
The coroutine implementation uses the std::bind function instead of directly passing the function and its parameters.
The first parameter of this std::bind function is the function the coroutine needs to run.
The second optional parameter is the class instance of this function. It is only required when the function belongs to a class and is not static.
The next parameter is a placeholder parameter, filled using std::placeholder::_1.
The coroutine itself will use this placeholder to fill in the added parameter of the opposite coroutine type that needs to be added to the function.
After these required parameters, any additional parameters the function might need are added.
Although the code needed to create a coroutine looks more complex than the same code for fibers, the main components remain the same, just placed within the std::bind function.

Second, coroutines are called instead of joined.
Since coroutines do not have a scheduler, they do not use the join function.
Instead, the programmer calls the coroutine itself as if it were a function call.
Coroutines cannot be called after they are finished, as this will result in an error.
Coroutines offer the functionality to check if they are finished or not.
This check is done using the boolean operator.
This operator will return true if the coroutine can still be called.

Third, the additional coroutine parameter will be called to yield the coroutine. Instead of using boost::this_fiber::yield() like fibers to yield the coroutine in this example, yield() will be called because the added coroutine parameter is called yield.
When changing the name of this parameter, all 'yield' calls will also need to be renamed.

The following code creates a coroutine running a non-static function that is part of a class.

{% highlight c++ %}
void StackfulCoroutinesBasics::memberExample() {
    std::cout << "memberExample: started running" << std::endl;

    std::cout << "memberExample: started creating a new push-type coroutine" << std::endl;
    boost::coroutines2::coroutine<void>::push_type myPushCoroutine(std::bind(&StackfulCoroutinesBasics::memberFunction, this, std::placeholders::_1));
    //StackfulCoroutinesBasics instance = StackfulCoroutinesBasics();
    //boost::coroutines2::coroutine<void>::push_type myPushCoroutine(std::bind(&StackfulCoroutinesBasics::memberFunction, &instance, std::placeholders::_1));
    std::cout << "memberExample: finished creating a new push-type coroutine" << std::endl;

    std::cout << "memberExample: started first push-type coroutine call" << std::endl;
    myPushCoroutine();
    std::cout << "memberExample: finished first push-type coroutine call" << std::endl;

    std::cout << "memberExample: finished running" << std::endl;
}

void StackfulCoroutinesBasics::memberFunction(boost::coroutines2::coroutine<void>::pull_type& yield)
{
    std::cout << "memberFunction: started running" << std::endl;
    //Do something
    std::cout << "memberFunction: finished running" << std::endl;
}

// --- Output ---
// memberExample: started running
// memberExample: started creating a new push-type coroutine
// memberExample: finished creating a new push-type coroutine
// memberExample: started first push-type coroutine call
// memberFunction: started running
// memberFunction: finished running
// memberExample: finished first push-type coroutine call
// memberExample: finished running
// --- Output ---
{% endhighlight %}

As stated, any required parameters can be added after the required placeholder parameter is in the std::bind function.
Adding parameters is worked out in the following example:

{% highlight c++ %}
void StackfulCoroutinesBasics::parameterExample() {
    std::cout << "parameterExample: started running" << std::endl;

    std::cout << "parameterExample: started creating a new push-type coroutine" << std::endl;
    boost::coroutines2::coroutine<void>::push_type myPushCoroutine(std::bind(parameterFunction, std::placeholders::_1, "String 1", "String 2"));
    //StackfulCoroutinesBasics instance = StackfulCoroutinesBasics();
    //boost::coroutines2::coroutine<void>::push_type myPushCoroutine(std::bind(parameterFunction, std::placeholders::_1, "String 1", "String 2"));
    std::cout << "parameterExample: finished creating a new push-type coroutine" << std::endl;

    std::cout << "parameterExample: started first push-type coroutine call" << std::endl;
    myPushCoroutine();
    std::cout << "parameterExample: finished first push-type coroutine call" << std::endl;

    std::cout << "parameterExample: finished running" << std::endl;
}

void StackfulCoroutinesBasics::parameterFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::string firstString, std::string secondString) {
    std::cout << "parameterFunction: started running" << std::endl;
    std::cout << "parameterFunction: the first string is " << firstString << std::endl;
    std::cout << "parameterFunction: the second string is " << secondString << std::endl;
    std::cout << "parameterFunction: finished running" << std::endl;
}

// --- Output ---
// parameterExample: started running
// parameterExample: started creating a new push-type coroutine
// parameterExample: finished creating a new push-type coroutine
// parameterExample: started first push-type coroutine call
// parameterFunction: started running
// parameterFunction: the first string is String 1
// parameterFunction: the second string is String 2
// parameterFunction: finished running
// parameterExample: finished first push-type coroutine call
// parameterExample: finished running
// --- Output ---
{% endhighlight %}

A feature of stackful coroutines not present in stackless coroutines is nested yields.
Because a stackful coroutine uses a stack to keep track of its state, it can yield inside any function that is called.
The following example demonstrates this principle.
The coroutine runs the main function.
This main function calls a sub-function and passes it the coroutine yield parameter.
The sub-function yield will work the same way as a yield inside the main coroutine function.
When the coroutine is called again, it will resume from inside the sub-function. 
This behaviour is one of the main benefits stackful coroutines have over stackless coroutines, even though they use more memory than their stackless counterpart.
Being able to yield the coroutine outside of the main body allows it to work with non-coroutine functions and have them yield.
As mentioned, stackless coroutines do not have this feature.
It might be possible to recreate similar behaviour with only stackless coroutines, but this would require all sub-functions to be coroutines, which might not be optimal depending on the use case [4, p. 10], [5, p. 7], [6, pp. 5-6].

{% highlight c++ %}
void StackfulCoroutinesBasics::nestedYieldExample() {
    std::cout << "nestedYieldExample: started running" << std::endl;

    boost::coroutines2::coroutine<void>::push_type myPushCoroutine(std::bind(mainYieldFunction, std::placeholders::_1));

    std::cout << "nestedYieldExample: started first push-type coroutine call" << std::endl;
    myPushCoroutine();
    std::cout << "nestedYieldExample: finished first push-type coroutine call" << std::endl;

    std::cout << "nestedYieldExample: started second push-type coroutine call" << std::endl;
    myPushCoroutine();
    std::cout << "nestedYieldExample: finished second push-type coroutine call" << std::endl;

    std::cout << "nestedYieldExample: started third push-type coroutine call" << std::endl;
    myPushCoroutine();
    std::cout << "nestedYieldExample: finished third push-type coroutine call" << std::endl;

    std::cout << "nestedYieldExample: finished running" << std::endl;
}

void StackfulCoroutinesBasics::mainYieldFunction(boost::coroutines2::coroutine<void>::pull_type& yield)
{
    std::cout << "mainYieldFunction: started running" << std::endl;

    std::cout << "mainYieldFunction: started first sub-function call" << std::endl;
    subYieldFunction(yield);
    std::cout << "mainYieldFunction: finished first sub-function call" << std::endl;

    std::cout << "mainYieldFunction: started second sub-function call" << std::endl;
    subYieldFunction(yield);
    std::cout << "mainYieldFunction: finished second sub-function call" << std::endl;

    std::cout << "mainYieldFunction: finished running" << std::endl;
}

void StackfulCoroutinesBasics::subYieldFunction(boost::coroutines2::coroutine<void>::pull_type& yield)
{
    std::cout << "subYieldFunction: started running" << std::endl;

    std::cout << "subYieldFunction: started first yield" << std::endl;
    yield();
    std::cout << "subYieldFunction: finished first yield" << std::endl;

    std::cout << "subYieldFunction: finished running" << std::endl;
}

// --- Output ---
// nestedYieldExample: started running
// nestedYieldExample: started first push-type coroutine call
// mainYieldFunction: started running
// mainYieldFunction: started first sub-function call
// subYieldFunction: started running
// subYieldFunction: started first yield
// nestedYieldExample: finished first push-type coroutine call
// nestedYieldExample: started second push-type coroutine call
// subYieldFunction: finished first yield
// subYieldFunction: finished running
// mainYieldFunction: finished first sub-function call
// mainYieldFunction: started second sub-function call
// subYieldFunction: started running
// subYieldFunction: started first yield
// nestedYieldExample: finished second push-type coroutine call
// nestedYieldExample: started third push-type coroutine call
// subYieldFunction: finished first yield
// subYieldFunction: finished running
// mainYieldFunction: finished second sub-function call
// mainYieldFunction: finished running
// nestedYieldExample: finished third push-type coroutine call
// nestedYieldExample: finished running
// --- Output ---

{% endhighlight %}

### References
[1] O. Kowalke, "Boost C++ Libraries Coroutine2," boost.org. Accessed: May 21, 2024. [Online.] Available:
[https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/index.html](https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/index.html)

[2] O. Kowalke, "Boost C++ Libraries Coroutine2 - coroutine<>::push_type," boost.org. Accessed: May 21, 2024. [Online.] Available:
[https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/coroutine2/coroutine/asymmetric/push_coro.html](https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/coroutine2/coroutine/asymmetric/push_coro.html)

[3] O. Kowalke, "Boost C++ Libraries Coroutine2 - coroutine<>::pull_type," boost.org. Accessed: May 21, 2024. [Online.] Available:
[https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/coroutine2/coroutine/asymmetric/pull_coro.html](https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/coroutine2/coroutine/asymmetric/pull_coro.html)

[4] G. Nishanov, "Fibers under the magnifying glass," ISO/IEC JTC1/SC22/WG21, Tech. Rep. N4397, April 2015. [Online]. Available: [https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4397.pdf](https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4397.pdf)

[5] O. Kowalke, "A low-level API for stackful coroutines," ISO/IEC JTC1/SC22/WG21, Tech. Rep. P1364R0, November 2018. [Online]. Available: [https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1364r0.pdf](https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1364r0.pdf)

[6] A. L. D. Moura and R. Ierusalimschy, “Revisiting coroutines,” ACM Transactions on Programming Languages and Systems, vol. 31, no. 2, pp. 6:1–6:31, February 2009. [Online]. Available: [https://dl.acm.org/doi/10.1145/1462166.1462167](https://dl.acm.org/doi/10.1145/1462166.1462167)