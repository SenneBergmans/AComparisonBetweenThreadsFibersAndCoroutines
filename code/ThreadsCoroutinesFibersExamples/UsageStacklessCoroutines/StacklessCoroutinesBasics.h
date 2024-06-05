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