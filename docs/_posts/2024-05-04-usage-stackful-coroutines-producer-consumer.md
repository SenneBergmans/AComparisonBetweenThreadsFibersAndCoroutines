---
layout: post
title:  "Usage - Stackful Coroutines - Producer-consumer"
date:   2024-05-04 11:00:00 +0200
categories: bachelor_thesis usage coroutines stackfull_coroutines producer-consumer
permalink: "/bachelor_thesis/usage/coroutines/stackful/producer-consumer"
---
This page will work out the same producer-consumer example as the threads and fibers producer-consumer pages.
The code from the fiber page will be used as a starting point, and the code will be changed to use the Boost Coroutine2 library [1].
The main function changes the boost::fibers::fiber type for boost::coroutine2::coroutine\<void\>::push_type [2].
Because the basic example does not require any variables to be passed to the coroutine or returned from it while it is running, the coroutine type will be void.
For the same reasons, both push-type and pull-type coroutines will work to create the example [2], [3].
For this example, push-type coroutines were chosen because they only run when called, and this is the easiest-to-understand order of execution in the code.
The join functions will be replaced with a while loop that will alternate the coroutine calls until all are finished.
This while loop was shown on the page [Differences - Coroutine - Fiber](/AComparisonBetweenThreadsFibersAndCoroutines{% link _posts/2024-05-01-explanation-differences-coroutine-fiber.md %}). 
An additional parameter will be inserted as the first parameter in both the producer and consumer functions. This parameter will be the required coroutine parameter used to yield.
The fiber yield statements will also be replaced with a call to the yield parameter.
These changes will result in the following code.

{% highlight c++ %}
#include <boost/coroutine2/all.hpp>
#include <iostream>

void producerFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::vector<int>* numberVector, int producedConsumedAmount, int index) {
    int counter = 0;
    int maxVectorSize = 5;
    while (counter < producedConsumedAmount) {
        if (numberVector->size() < maxVectorSize) {
            numberVector->push_back(counter);
            std::cout << "Producer " << index << " produced: " << counter << std::endl;
            counter++;
        }
        else {
            std::cout << "Producer " << index << " did nothing, numberVector is full" << std::endl;
            //Yield the producer when the vector is full.
            yield();
        }
        //Alternative placement of the yield statement causing a yield after every item produced.
        //yield();
    }
}
void consumerFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::vector<int>* numberVector, int producedConsumedAmount, int index) {
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
            yield();
        }
        //Alternative placement of the yield statement causing a yield after every item consumed.
        //yield();
    }
}

int main() {
    std::vector<int> numberVector = std::vector<int>();
    int producedConsumedAmount = 10;

    boost::coroutines2::coroutine<void>::push_type producerCoroutine(std::bind(producerFunction, std::placeholders::_1, &numberVector, producedConsumedAmount, 0));
    boost::coroutines2::coroutine<void>::push_type consumerCoroutine(std::bind(consumerFunction, std::placeholders::_1, &numberVector, producedConsumedAmount, 1));

    bool stillActive = true;
    while (stillActive) {
        stillActive = false;
        if (producerCoroutine) {
            producerCoroutine();
            stillActive = true;
        }
        if (consumerCoroutine) {
            consumerCoroutine();
            stillActive = true;
        }
    }
}

// --- Output ---
// Producer 0 produced: 0
// Producer 0 produced: 1
// Producer 0 produced: 2
// Producer 0 produced: 3
// Producer 0 produced: 4
// Producer 0 did nothing, numberVector is full
// Consumer 1 consumed: 4
// Consumer 1 consumed: 3
// Consumer 1 consumed: 2
// Consumer 1 consumed: 1
// Consumer 1 consumed: 0
// Consumer 1 did nothing, numberVector is empty
// Producer 0 produced: 5
// Producer 0 produced: 6
// Producer 0 produced: 7
// Producer 0 produced: 8
// Producer 0 produced: 9
// Consumer 1 consumed: 9
// Consumer 1 consumed: 8
// Consumer 1 consumed: 7
// Consumer 1 consumed: 6
// Consumer 1 consumed: 5
// --- Output ---
{% endhighlight %}

Since coroutines do not have a scheduler, their order of creation does not affect how they are sorted inside the scheduler.
The easiest way to extend the example to multiple producers and consumers would be to save all coroutines in a vector and have the while-loop ensure all coroutines finish before the main function ends.
When this implementation is used, the coroutines' order inside the vector will have the same impact as the order of fibers inside the scheduler.
The coroutines can be added to the vector using the same method that alternates the fibers.
Any other algorithm that ensures producer and consumer coroutines alternate will also work.
The following example uses the same technique used in the fiber example:

{% highlight c++ %}
int main() {
    std::vector<int> numberVector = std::vector<int>();
    std::vector<boost::coroutines2::coroutine<void>::push_type> coroutineVector{};
    int producerAmount = 5;
    int consumerAmount = 5;
    int producedConsumedAmount = 10;


    int largestCounter = producerAmount;
    if (producerAmount < consumerAmount) {
        largestCounter = consumerAmount;
    }

    int remainingProducerInput = producedConsumedAmount;
    int remainingProducers = producerAmount;
    int remainingConsumerInput = producedConsumedAmount;
    int remainingConsumers = consumerAmount;

    for (int i = 0; i < largestCounter; i++) {
        if (i < producerAmount) {
            int toProduceAmount = (remainingProducerInput / remainingProducers);
            boost::coroutines2::coroutine<void>::push_type producerCoroutine(std::bind(producerFunction, std::placeholders::_1, &numberVector, toProduceAmount, i));
            coroutineVector.push_back(std::move(producerCoroutine));
            remainingProducerInput = remainingProducerInput - toProduceAmount;
            remainingProducers = remainingProducers - 1;
        }
        if (i < consumerAmount) {
            int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
            boost::coroutines2::coroutine<void>::push_type consumerCoroutine(std::bind(consumerFunction, std::placeholders::_1, &numberVector, toConsumeAmount, (producerAmount + i)));
            coroutineVector.push_back(std::move(consumerCoroutine));
            remainingConsumerInput = remainingConsumerInput - toConsumeAmount;
            remainingConsumers = remainingConsumers - 1;
        }
    }

    bool stillRunning = true;

    while (stillRunning) {
        stillRunning = false;
        for (int i = 0; i < coroutineVector.size(); i++) {
            if (coroutineVector.at(i)) {
                coroutineVector.at(i)();
                stillRunning = true;
            }
        }
    }
}

// --- Output ---
// Producer 0 produced: 0
// Producer 0 produced: 1
// Consumer 5 consumed: 1
// Consumer 5 consumed: 0
// Producer 1 produced: 0
// Producer 1 produced: 1
// Consumer 6 consumed: 1
// Consumer 6 consumed: 0
// Producer 2 produced: 0
// Producer 2 produced: 1
// Consumer 7 consumed: 1
// Consumer 7 consumed: 0
// Producer 3 produced: 0
// Producer 3 produced: 1
// Consumer 8 consumed: 1
// Consumer 8 consumed: 0
// Producer 4 produced: 0
// Producer 4 produced: 1
// Consumer 9 consumed: 1
// Consumer 9 consumed: 0
// --- Output ---
{% endhighlight %}

### References
[1] O. Kowalke. "Chapter 1. Coroutine2." boost.org. Accessed: Feb. 13, 2024. [Online.] Available:
[https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/index.html](https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/index.html)

[2] O. Kowalke. "Chapter 1. Coroutine2 - coroutine<>::push_type." boost.org. Accessed: Feb. 13, 2024. [Online.] Available:
[https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/coroutine2/coroutine/asymmetric/push_coro.html](https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/coroutine2/coroutine/asymmetric/push_coro.html)

[3] O. Kowalke. "Chapter 1. Coroutine2 - coroutine<>::pull_type." boost.org. Accessed: Feb. 13, 2024. [Online.] Available:
[https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/coroutine2/coroutine/asymmetric/pull_coro.html](https://www.boost.org/doc/libs/1_85_0/libs/coroutine2/doc/html/coroutine2/coroutine/asymmetric/pull_coro.html)