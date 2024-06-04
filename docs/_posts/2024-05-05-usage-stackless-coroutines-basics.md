---
layout: post
title:  "Usage - Stackless Coroutines - Basics"
date:   2024-05-05 10:00:00 +0200
categories: usage coroutines stackless_coroutines basics
permalink: "/usage/coroutines/stackless/basics"
---
This page covers the basic usage of stackless coroutines.
The coroutines that are part of the C++ 20 standard will be used to implement stackless coroutines.
The page goes over the basic usage of stackless coroutines and explains the different ways the C++ 20 standard implementation can be used.
Readers familiar with coroutines and the differences between stackful and stackless coroutines might find starting with the producer-consumer page covering stackless coroutines more useful.

Any function in C++ 20 that uses co_yield, co_await or co_return is considered a coroutine.
These coroutines require a particular return type that will be used to handle calls to the coroutine.
In most use cases, it is possible to use a library that handles the return type to make working with coroutines easier.
Another option is to create a custom return type that the coroutines can use.
This custom return type was chosen during the research of the topic.
This choice was made because it offered complete control over the behaviour of the coroutine and helped get a better understanding of the inner workings of C++ stackless coroutines.
In this basic example, the return type of all coroutines will always be CoroutineHandler.
CoroutineHandler is a custom-written class based on the example provided in the documentation on the website cppreference [1].
The return type is not discussed on this page because it requires a more in-depth explanation than fits the basics page.
The full explanation of the CoroutineHandler class can be found on the Stackless coroutines - CoroutineHandler page.
The only important thing about this CoroutineHandler class to understand the basic examples is that this return type can be saved when creating a coroutine and will be used to call the coroutine.

A custom class containing all the example functions will be used.
This usage of a class is done because the creation of coroutines works differently for static and non-static functions.
When the function the coroutine should run is not part of any class, the same code as the example for static functions can be used.
The class is defined as follows in the header file: 

{% highlight c++ %}
#pragma once

#include <string>
#include <coroutine>
#include "CoroutineHandler.h"

class StacklessCoroutinesBasics
{
public:
    static void staticExample();
    void memberExample();
    static void parameterExample();

private:
    static CoroutineHandler<bool> staticFunction();
    CoroutineHandler<bool> memberFunction();
    static CoroutineHandler<bool> parameterFunction(std::string firstString, std::string secondString);
};
{% endhighlight %}


The basic example will create a coroutine and call it.
The main function will create the coroutine. 
Coroutine are created by calling the coroutine like a function and saving the return value using the CoroutineHandler class.
The coroutine itself will have a return type of CoroutineHandler.
Inside the body of the coroutine, one of three statements must be present at least once for the function to become a coroutine.
These statements are co_yield, co_await and co_return.
In this example, the co_yield statement was used.
Keep in mind that the co_yield statement always expects a parameter.
The type expected by the CoroutineHandler in this example is a boolean.
The co_yield will thus use the value true as its parameter.
{% highlight c++ %}
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

// --- Output ---
// staticExample: started running
// staticExample: started creating a new coroutine
// staticExample: finished creating a new coroutine
// staticExample: started first coroutine call
// staticFunction: started running
// staticFunction: start yield
// staticExample: finished first coroutine call
// staticExample: started second coroutine call
// staticFunction: finished yield
// staticFunction: finished running
// staticExample: finished second coroutine call
// staticExample: finished running
// --- Output ---
{% endhighlight %}

Creating a non-static coroutine works the same as calling a non-static function.
{% highlight c++ %}
void StacklessCoroutinesBasics::memberExample() {
    std::cout << "memberExample: started running" << std::endl;

    std::cout << "memberExample: started creating a new coroutine" << std::endl;
    CoroutineHandler<bool> myCoroutine = memberFunction();
    //StacklessCoroutinesBasics instance = StacklessCoroutinesBasics();
    //CoroutineHandler<bool> myCoroutine = instance.memberFunction();
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

// --- Output ---
// memberExample: started running
// memberExample: started creating a new coroutine
// memberExample: finished creating a new coroutine
// memberExample: started first coroutine call
// memberFunction: started running
// memberFunction: finished running
// memberExample: finished first coroutine call
// memberExample: finished running
// --- Output ---
{% endhighlight %}

Adding parameters to the coroutines also works the same way as adding parameters to a function.
{% highlight c++ %}
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

// --- Output ---
// parameterExample: started running
// parameterExample: started creating a new coroutine
// parameterExample: finished creating a new coroutine
// parameterExample: started first coroutine call
// parameterFunction: started running
// parameterFunction: the first string is String 1
// parameterFunction: the second string is String 2
// parameterFunction: finished running
// parameterExample: finished first coroutine call
// parameterExample: finished running
// --- Output ---
{% endhighlight %}

### References
[1] "Coroutines (C++ 20)" cppreference.com. Accessed: May 23, 2024. [Online.] Available:
[https://en.cppreference.com/w/cpp/language/coroutines](https://en.cppreference.com/w/cpp/language/coroutines)