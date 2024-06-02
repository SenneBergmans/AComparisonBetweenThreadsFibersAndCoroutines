---
layout: post
title:  "Usage - Threads - Producer-consumer"
date:   2024-05-02 11:00:00 +0200
categories: usage threads
permalink: "/usage/threads/producer-consumer"
---
This page works out the producer-consumer example using Boost threads [1]. The main goal of this page is to allow for a comparison between this code and the producer-consumer examples of fibers and threads.

The producer thread will produce 25 numbers and push these numbers to the number vector.
The consumer thread will take 25 numbers from the number vector.
Several hard-coded parameters are added to the functions to create a simple example that works in a single file.
The actual GitHub page linked to this GitHub pages site contains the example using multiple classes to avoid hard-coding the parameters.

The main function starts by creating the vector that will store the numbers, a mutex and the producer and consumer threads.
After this is done, it joins both threads to ensure they complete fully before the main function ends.
The producer thread runs the producer function.
As long as this thread still has numbers to produce, it will lock the vector to ensure the consumer does not access it simultaneously.
After that, the producer checks if the vector still has space to produce numbers.
If this is the case, it will produce a number.
The producer repeats these actions until it has produced the predefined number of numbers. 
The consumer thread runs the consumerFunction.
If the consumer thread still has numbers to consume, it will lock the vector to ensure the producer does not access it simultaneously.
It checks if the vector contains numbers.
If this is the case, it will consume those numbers.
The consumer repeats these actions until the consumer finishes.

{% highlight c++ %}
void producerFunction(std::vector<int>* numberVector, boost::mutex* threadMutex) {
    int counter = 0;
    int maxVectorSize = 5;
    int producedConsumedAmount = 25;

    while (counter < producedConsumedAmount) {
        boost::lock_guard<boost::mutex> guard(*threadMutex); //Lock mutex before using the numberVector
        if (numberVector->size() < maxVectorSize) {
            numberVector->push_back(counter);
            std::cout << "Producer produced: " << counter << std::endl;
            counter++;
        }
        else {
            //std::cout << "Producer did nothing, numberVector is full" << std::endl;
        }
    }
}
void consumerFunction(std::vector<int>* numberVector, boost::mutex* threadMutex) {
    int counter = 0;
    int producedConsumedAmount = 25;

    while (counter < producedConsumedAmount) {
        boost::lock_guard<boost::mutex> guard(*threadMutex); //Lock mutex before using the numberVector
        if (!numberVector->empty()) {
            int number = numberVector->back();
            numberVector->pop_back();
            counter++;
            std::cout << "Consumer consumed: " << number << std::endl;
        }
        else {
            //std::cout << "Consumer did nothing, numberVector is empty" << std::endl;
        }
    }
}

int main() {
    std::vector<int> numberVector = std::vector<int>();
    boost::mutex threadMutex;

    boost::thread producerThread(producerFunction, &numberVector, &threadMutex);
    boost::thread consumerThread(consumerFunction, &numberVector, &threadMutex);

    producerThread.join();
    consumerThread.join();
}
{% endhighlight %}

The lines printing when the producer or consumer can not do anything are commented out.
This is a result of thread using preemptive scheduling.
Because an external scheduler decides when a context switch happens, a thread can stay stuck in the case where it cannot produce or consume without the scheduler changing the active thread.
Continuously looping in the else case would make the output less readable which is why it is commented out.
Please also keep in mind that threads are not deterministic because they use preemptive scheduling and an external scheduler.
Because of this each run of the example might result in a different output.

### References

[1] A. Williams and V. J. Botet Escriba, "Boost C++ Libraries Thread
4.8.0," boost.org, Accessed: Mar. 5, 2024. [Online]. Available:
[https://www.boost.org/doc/libs/1_85_0/doc/html/thread.html](https://www.boost.org/doc/libs/1_85_0/doc/html/thread.html)

[2] S. Bergmans, “Complete code examples for A Comparison between Threads,
Fibers and Coroutines for Developing Concurrent Software,” github.com,
Accessed: Jun. 2, 2024. [Online]. Available: [https://github.com/
SenneBergmans/AComparisonBetweenThreadsFibersAndCoroutines](https://github.com/
SenneBergmans/AComparisonBetweenThreadsFibersAndCoroutines)