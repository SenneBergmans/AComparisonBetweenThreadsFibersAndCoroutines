---
layout: post
title:  "Usage - Fibers - Producer-consumer"
date:   2024-05-03 11:00:00 +0200
categories: usage fibers producer-consumer
permalink: "/usage/fibers/producer-consumer"
---
This page works out the producer-consumer example using fibers.
It uses the producer-consumer example, which is explained using threads as a starting point, and changes the example to use fibers.
The aim of doing this is to offer a good comparison between the usage of the different technologies.
It will also go over several methods of scheduling producers and consumers and their impact on how the code runs.

The main function remains the same, only changing the boost::thread type for the boost::fibers::fiber type [1]. 
The mutex is also removed.
It can be removed because the example will run the two fibers on a single thread.
Because fibers only work concurrently and not parallel, it is unnecessary to lock the vector. 
The producer and consumer functions require two changes.
First, the lock statements can be removed because the mutex is no longer used.
Second, a yield statement must be added to make the fiber yield.
Without yield statements, the first producer called will stay in the else statement, not progressing the while loop.
There are also two possible placements when adding the yield statement.
The yield statement could be added as the last line of the while loop or inside the else statement.
When the last statement of the while loop is the yield statement, the fiber will yield after every single item produced/consumed. 
Should the yield statement be added inside of the else case, then the fiber will only yield once the vector is full or empty.
The second option seems preferable in this case.
In the example, there is no real need to ensure the two fibers switch as often as possible, and every added context switch takes time. 
Only yielding when the vector is full or empty reduces the yield statements as much as possible.
Waiting any longer than when the vector is full or empty also has no benefit because the active fiber can no longer progress.
The amount that needs to be produced will be provided as a parameter to allow for an extension of the example using multiple fibers required to illustrate a future point.
The code will thus be changed as follows:

{% highlight c++ %}
#ifndef NOMINMAX //Reference [2] (https://stackoverflow.com/a/6884251)
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>
#include <iostream>
#include <vector>

void producerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index) {
    int counter = 0;
    int maxVectorSize = 2;

    while (counter < producedConsumedAmount) {
        if (numberVector->size() < maxVectorSize) {
            numberVector->push_back(counter);
            std::cout << "Producer " << index << " produced: " << counter << std::endl;
            counter++;
        }
        else {
            std::cout << "Producer " << index << " did nothing, numberVector is full" << std::endl;
            //Yield the producer when the vector is full.
            boost::this_fiber::yield();
        }
        //Alternative placement of the yield statement causing a yield after every item produced.
        //boost::this_fiber::yield();
    }
}
void consumerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index) {
    int counter = 0;

    while (counter < producedConsumedAmount) {
        if (!numberVector->empty()) {
            int number = numberVector->back();
            numberVector->pop_back();
            counter++;
            std::cout << "Consumer " << index << " consumed: " << number << std::endl;
        }
        else {
            std::cout << "Consumer " << index << " did nothing, numberVector is empty" << std::endl;
            //Yield the consumer when all numbers have been consumed.
            boost::this_fiber::yield();
        }
        //Alternative placement of the yield statement causing a yield after every item consumed.
        //boost::this_fiber::yield();
    }
}

int main() {
    std::vector<int> numberVector = std::vector<int>();
    int producedConsumedAmount = 6;

    boost::fibers::fiber producerFiber(producerFunction, &numberVector, producedConsumedAmount, 0);
    boost::fibers::fiber consumerFiber(consumerFunction, &numberVector, producedConsumedAmount, 1);

    producerFiber.join();
    consumerFiber.join();
}

// --- Output ---
// Producer 0 produced: 0
// Producer 0 produced: 1
// Producer 0 did nothing, numberVector is full
// Consumer 1 consumed: 1
// Consumer 1 consumed: 0
// Consumer 1 did nothing, numberVector is empty
// Producer 0 produced: 2
// Producer 0 produced: 3
// Producer 0 did nothing, numberVector is full
// Consumer 1 consumed: 3
// Consumer 1 consumed: 2
// Consumer 1 did nothing, numberVector is empty
// Producer 0 produced: 4
// Producer 0 produced: 5
// Consumer 1 consumed: 5
// Consumer 1 consumed: 4
// --- Output ---
{% endhighlight %}

The given example will work to solve the task set out in the example.
However, this implementation could cause inefficiency when the example uses multiple fibers.
In the example, the producer is created before the consumer. Should a programmer naively extend this code as is done in the following example, a problem arises. The example will split the total items that must be produced over a predefined number of producers and consumers.

{% highlight c++ %}
int main() {
    int producerAmount = 3;
    int consumerAmount = 3;
    int producedConsumedAmount = 6;

    std::vector<int> numberVector = std::vector<int>();
    std::vector<boost::fibers::fiber> fiberVector(producerAmount + consumerAmount);

    int remainingProducerInput = producedConsumedAmount;
    int remainingProducers = producerAmount;
    int remainingConsumerInput = producedConsumedAmount;
    int remainingConsumers = consumerAmount;

    for (int i = 0; i < producerAmount; i++) {
        int toProduceAmount = (remainingProducerInput / remainingProducers);
        fiberVector[i] = boost::fibers::fiber(producerFunction, &numberVector, toProduceAmount, i);
        remainingProducerInput = remainingProducerInput - toProduceAmount;
        remainingProducers = remainingProducers - 1;
    }

    for (int j = 0; j < consumerAmount; j++) {
        int toProduceAmount = (remainingConsumerInput / remainingConsumers);
        fiberVector[producerAmount + j] = boost::fibers::fiber(consumerFunction, &numberVector, toProduceAmount, (producerAmount + j));
        remainingConsumerInput = remainingConsumerInput - toProduceAmount;
        remainingConsumers = remainingConsumers - 1;
    }

    for (int i = 0; i < fiberVector.size(); i++) {
        fiberVector[i].join();
    }
}

// --- Output ---
// Producer 0 produced: 0
// Producer 0 produced: 1
// Producer 1 did nothing, numberVector is full
// Producer 2 did nothing, numberVector is full
// Consumer 3 consumed: 1
// Consumer 3 consumed: 0
// Consumer 4 did nothing, numberVector is empty
// Consumer 5 did nothing, numberVector is empty
// Producer 1 produced: 0
// Producer 1 produced: 1
// Producer 2 did nothing, numberVector is full
// Consumer 4 consumed: 1
// Consumer 4 consumed: 0
// Consumer 5 did nothing, numberVector is empty
// Producer 2 produced: 0
// Producer 2 produced: 1
// Consumer 5 consumed: 1
// Consumer 5 consumed: 0
// --- Output ---
{% endhighlight %}

In this example, only the first producer produces any items, and only the first consumer consumes any items due to the ordering of the producers and consumers.
The fiber scheduler uses the round-robin algorithm by default to decide the next fiber to call [3]. 
This algorithm ensures fibers are called in the order they are created.
When the first producer is called and fills the vector, it will be followed by another producer who cannot do anything because the vector is already full.
The same thing will happen for the consumers.
The first consumer empties the vector, and all subsequent consumers will be called on an empty vector.
It is easy to solve this problem, but the programmer must know that ordering needs to be given proper thought.
The solution for this example would be to alternate the creation of producers and consumers, as shown in the following code.


{% highlight c++ %}
int main() {
    int producerAmount = 3;
    int consumerAmount = 3;
    int producedConsumedAmount = 6;

    std::vector<int> numberVector = std::vector<int>();
    std::vector<boost::fibers::fiber> fiberVector(producerAmount + consumerAmount);

    int largestCounter = producerAmount;
    if (producerAmount < consumerAmount) largestCounter = consumerAmount;

    int remainingProducerInput = producedConsumedAmount;
    int remainingProducers = producerAmount;
    int remainingConsumerInput = producedConsumedAmount;
    int remainingConsumers = consumerAmount;

    for (int i = 0; i < largestCounter; i++) {
        if (i < producerAmount) {
            int toProduceAmount = (remainingProducerInput / remainingProducers);
            fiberVector[i] = boost::fibers::fiber(producerFunction, &numberVector, toProduceAmount, i);
            remainingProducerInput = remainingProducerInput - toProduceAmount;
            remainingProducers = remainingProducers - 1;
        }
        if (i < consumerAmount) {
            int toProduceAmount = (remainingConsumerInput / remainingConsumers);
            fiberVector[producerAmount + i] = boost::fibers::fiber(consumerFunction, &numberVector, toProduceAmount, (producerAmount + i));
            remainingConsumerInput = remainingConsumerInput - toProduceAmount;
            remainingConsumers = remainingConsumers - 1;
        }
    }

    for (int i = 0; i < fiberVector.size(); i++) {
        fiberVector[i].join();
    }
}

// --- Output ---
// Producer 0 produced: 0
// Producer 0 produced: 1
// Consumer 3 consumed: 1
// Consumer 3 consumed: 0
// Producer 1 produced: 0
// Producer 1 produced: 1
// Consumer 4 consumed: 1
// Consumer 4 consumed: 0
// Producer 2 produced: 0
// Producer 2 produced: 1
// Consumer 5 consumed: 1
// Consumer 5 consumed: 0
// --- Output ---
{% endhighlight %}

By alternating the creation of producers and consumers, their order in the scheduler is better, and every producer and consumer can do work when it is called.

### References
[1] O. Kowalke. "Boost C++ Libraries Fiber." boost.org. Accessed: May 13, 2024. [Online.] Available: [https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/index.html](https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/index.html)

[2] Zitrax and H. Passant. "Answer to warning C4003: not enough actual parameters for macro 'max' - Visual Studio 2010 C++." stackoverflow.com. Accessed: Mar. 1, 2024. [Online.] Available: [https://stackoverflow.com/a/6884251](https://stackoverflow.com/a/6884251)

[3] O. Kowalke. "Boost C++ Libraries Fiber Scheduling." boost.org. Accessed: May 13, 2024. [Online.] Available: [https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/fiber/scheduling.html](https://www.boost.org/doc/libs/1_85_0/libs/fiber/doc/html/fiber/scheduling.html)