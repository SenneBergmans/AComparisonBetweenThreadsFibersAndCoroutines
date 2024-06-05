#include "StackfulCoroutinesBasics.h"

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