---
layout: post
title:  "Testing - TimeUsage - General"
date:   2024-05-06 10:00:00 +0200
categories: testing TimeUsage
permalink: "/testing/TimeUsage/general"
description: "General"
---

As part of the research, several time usage tests were conducted to determine the performance difference between threads, fibers, and coroutines. 
This page will cover the general methodology and test code used in all tests to provide the needed information to repeat and check the test results mentioned in the paper.

Given the goal of testing four implementations of the same test, each using a different technology, a general TimeUsage class was created.
The purpose of this general TimeUsage class was to ensure that all tests performed were timed in the same way.
The TimeUsage class contains several helper functions that make testing easier.
The first is a testSoloFunction that takes an Rvalue reference of a function and returns a string of the time in seconds it took to run the function.

The second helper function is a virtual testFunctions function.
All tests will implement this function.
Each implementation will call testSoloFunction for all needed tests and return the results inside a vector containing string vectors.

The third helper function, runTests, calls the virtual testFunctions function implemented for each test.
After getting the results, the FileWriter class is used to write the results into a .csv file.

A fourth helper function, addTestInformation, adds general information about the test to the first vector containing strings.

These four functions are used to streamline the testing process.
Each of the test categories will inherit the TimeUsage function.
After implementing their specific testFunctions implementation, they can call runTests, and every test will be conducted, and the results will be exported to a .csv file.

All tests are divided into their test category.
The categories are ContextSwitch, ProducerConsumer, and Mergesort.
Each category will inherit the general TimeUsage, conduct specific tests for each technology, and combine the results.
Every test category uses some parameters specific to that category to compare the performance of the tests with varying parameters.
In addition to that, the GlobalTestParameter class defines parameters used by all tests. In this implementation, the only global parameter is the sample size, which defines the number of times each test is repeated.

The test pages will discuss the parameters specific to each of the tests and the test results from the research. 
The complete test code can be found in [this GitHub repository](https://github.com/SenneBergmans/AComparisonBetweenThreadsFibersAndCoroutines).