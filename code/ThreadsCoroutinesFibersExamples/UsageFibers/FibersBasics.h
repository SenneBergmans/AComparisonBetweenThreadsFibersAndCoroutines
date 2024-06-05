#pragma once

#include <iostream>

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
    static void parameterFunction(std::string firstString, std::string secondString);
    static void noYieldFunction(int number);
    static void yieldFunction(int number);
};