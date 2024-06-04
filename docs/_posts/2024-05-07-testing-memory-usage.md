---
layout: post
title:  "Testing - MemoryUsage"
date:   2024-05-07 10:00:00 +0200
categories: testing MemoryUsage
permalink: "/testing/MemoryUsage"
description: "Memory Usage"
---

Simple examples were made using all the technologies to test their memory usage.
The goal was to make an empty fiber and coroutine function to ensure that the memory usage measured is only the usage of the fiber or coroutine.
Using the heap profiling tool that is part of Visual Studio Community 2022, the memory usage of the technologies was then measured at ten different breakpoints.
The usage is measured on different breakpoints to visualize the moment memory is allocated and deallocated.
The test shows, for example, that Boost fibers will deallocate their memory when they are joined, while coroutines will only deallocate their memory when they are destroyed because their function goes out of scope.

{% highlight C++%}
void FibersMemoryUsage::emptyMemoryUsage() {
    //Breakpoint 1: before the function call
    //Breakpoint 2: after the function call
    boost::fibers::fiber fiber_1(emptyFunction);
    //Breakpoint 3: after the first fiber creation

    boost::fibers::fiber fiber_2(emptyFunction);
    //Breakpoint 4: after the second fiber creation

    boost::fibers::fiber fiber_3(emptyFunction);
    //Breakpoint 5: after the third fiber creation

    fiber_1.join();
    //Breakpoint 6: after the first join
    fiber_2.join();
    //Breakpoint 7: after the second join
    fiber_3.join();
    //Breakpoint 8: after the third join

    //Breakpoint 9: before the function ends
    //Breakpoint 10: after the function ends
}
{% endhighlight%}

In addition to the empty functions, additional examples were made containing, for example, parameters.
Additional parameters were added to see if adding parameters or variables to the fibers or coroutines would further increase dynamic memory usage.

Additional research was also done on removing any dynamic memory usage for stackless coroutines. References [1], [2], and [3] show methods like compiler optimizations that can remove dynamic memory usage in special cases.

[1] G. Nishanov, "Using Coroutine TS with zero dynamic allocations," ISO/IEC JTC1/SC22/WG21, Tech. Rep. P1365R0, November 2018. [Online]. Available: [https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1365r0.pdf](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1365r0.pdf)

[2] R. Smith and G. Nishanov, "Halo: coroutine Heap Allocation eLision Optimization: the joint response," ISO/IEC JTC1/SC22/WG21, Tech. Rep. P0981R0, March 2018. [Online]. Available: [https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0981r0.html](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0981r0.html)

[3] "Coroutines (C++20)," cppreference.com, Accessed: May 23, 2024. [Online]. Available: [https://en.cppreference.com/w/cpp/language/coroutines](https://en.cppreference.com/w/cpp/language/coroutines)