#include "TimeUsage.h"
#include "FileWriter.h"

//https://stackoverflow.com/questions/6884093/warning-c4003-not-enough-actual-parameters-for-macro-max-visual-studio-2010
#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>

#include <vector>
#include <string>
#include <chrono>
#include <iostream>

const std::vector<int> TimeUsage::_testingAmounts = { 10 };

void TimeUsage::runTests(std::string testName)
{
    std::vector<std::vector<std::string>> data = testFunctions(addTestInformation());
    FileWriter writer;
    writer.writeToNewCSV(data, testName);
}

std::vector<std::string> TimeUsage::addTestInformation() {
    std::vector<std::string> testInformation;

    //General information
    testInformation.push_back("Test class");
    testInformation.push_back("Test name");
    testInformation.push_back("Function name");

    return testInformation;
}

bool TimeUsage::showDebugPrint(int input)
{
    if (!_debugPrint) {
        return false;
    }
    if (input < _debugPrintThreshold) {
        return true;
    }

    return false;
}