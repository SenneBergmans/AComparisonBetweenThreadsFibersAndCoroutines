---
layout: post
title:  "Testing - TimeUsage - Context switch"
date:   2024-05-06 11:00:00 +0200
categories: testing TimeUsage
permalink: "/testing/TimeUsage/context-switch"
description: "Context switch"
---

The ContextSwitch test was implemented to understand each technology's context switch speed.
This test will create two instances of each technology and make them each yield X amount of times before finishing.
An important note to add to this test is that the different technologies may complete the test in different ways.
For example, the fiber scheduler will be called after each fiber yield, while the main function will be called when a stackful coroutine yields.
This test does not account for this difference.
The aim was to make the written code as similar as possible.

The general layout of the test can be seen using the fiber code as an example: 
{% highlight c++ %}
void ContextSwitchFibers::contextSwitch(int input)
{
	boost::fibers::fiber fiber1 = boost::fibers::fiber(&ContextSwitchFibers::contextSwitchFunction, input);
	boost::fibers::fiber fiber2 = boost::fibers::fiber(&ContextSwitchFibers::contextSwitchFunction, input);

	fiber1.join();
	fiber2.join();
}
{% endhighlight %}

Two fibers are created and joined immediately afterward.
Because coroutines do not use the join function, a while loop will loop over the coroutines until both are finished.
The context switch function contains a for loop that will loop a predefined number of times, and in each loop, it will yield doing nothing else.
For example: 
{% highlight c++ %}
void ContextSwitchFibers::contextSwitchFunction(int input)
{
	for (int i = 0; i < input; i++) {
		if (TimeUsage::showDebugPrint(input)) std::cout << boost::this_fiber::get_id() << " yielded " << std::endl;
 		boost::this_fiber::yield();
	}
}
{% endhighlight %}


The only variable parameter in this test was the amount of context switches that would happen in each function. In the test results mentioned in the paper, this parameter was chosen at 30 million.

The test was conducted for threads, fibers, stackful coroutines, and stackless coroutines.
Although the test was conducted using threads, the results are not included in the final results discussed in the paper.
Because threads use preemptive scheduling, they do not always yield when a yield statement is placed.
This behavior caused the thread example to continue looping in the for loop without context switching.
An alternative test was created that forced an actual context switch on each loop by only allowing the loop to continue once the second thread had been called using a boolean flag. 
This alternative performed much worse than the other tests.
When looking into the debug code, it could be seen that each thread stayed active for some time, even though it could not progress until the other thread had been switched in.
Because neither of the thread tests was comparable to the other tests, their results were not included.
The complete test code can be found in [this GitHub repository](https://github.com/SenneBergmans/AComparisonBetweenThreadsFibersAndCoroutines).