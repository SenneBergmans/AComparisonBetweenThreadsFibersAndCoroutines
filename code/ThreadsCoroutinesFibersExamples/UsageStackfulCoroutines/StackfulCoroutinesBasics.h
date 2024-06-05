#pragma once

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