#pragma once

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

