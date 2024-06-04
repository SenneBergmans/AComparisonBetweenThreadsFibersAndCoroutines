---
layout: post
title:  "Usage - Stackless Coroutines - Producer-consumer"
date:   2024-05-05 11:00:00 +0200
categories: usage coroutines stackless_coroutines
permalink: "/usage/coroutines/stackless/producer-consumer"
description: "Producer consumer"
---

This page will work out the same producer-consumer example as the stackful coroutine page.
The code from this page will be used as a starting point and changed to make use of stackless coroutines [1].
The main function changes the boost::coroutine2::coroutine<void>::push_type coroutines to CoroutineHandler<bool> coroutines.
Because the value returned when coroutines yield is not used, the boolean type is chosen for the coroutines.
The rest of the main function can stay the same.

In the producer and consumer functions, the additional parameter added by the stackful coroutines can be removed when switching to stackless coroutines.
The yield statement this parameter provided will be replaced with co_yield true.
The value passed to the co_yield function is not used in this example.
These changes will result in the following code:

{% highlight c++ %}
#include "CoroutineHandler"
#include <coroutine>
#include <iostream>
#include <vector>

CoroutineHandler<bool> producerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index)
{
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
            co_yield true;
        }
        //Alternative placement of the yield statement causing a yield after every item produced.
        //co_yield true;
    }
}

CoroutineHandler<bool> consumerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index)
{
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
            co_yield true;
        }
        //Alternative placement of the yield statement causing a yield after every item consumed.
        //co_yield true;
    }
}

int main()
{
    std::vector<int> numberVector = std::vector<int>();
    int producedConsumedAmount = 6;

    CoroutineHandler<bool> producerCoroutine = producerFunction(&numberVector, producedConsumedAmount, 0);
    CoroutineHandler<bool> consumerCoroutine = consumerFunction(&numberVector, producedConsumedAmount, 1);

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

The changes made to have multiple producers and consumers using stackful coroutines also work for stackless coroutines.
This results in the following code:

{% highlight c++ %}
int main() {
    std::vector<int> numberVector = std::vector<int>();
    std::vector<CoroutineHandler<bool>> coroutineVector{};
    int producerAmount = 3;
    int consumerAmount = 3;
    int producedConsumedAmount = 6;


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
            CoroutineHandler<bool> producerCoroutine = producerFunction(&numberVector, toProduceAmount, i);
            coroutineVector.push_back(std::move(producerCoroutine));
            remainingProducerInput = remainingProducerInput - toProduceAmount;
            remainingProducers = remainingProducers - 1;
        }
        if (i < consumerAmount) {
            int toConsumeAmount = (remainingConsumerInput / remainingConsumers);
            CoroutineHandler<bool> consumerCoroutine = consumerFunction(&numberVector, toConsumeAmount, (producerAmount + i));
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


### References
[1] "Coroutines (C++ 20)" cppreference.com. Accessed: May 23, 2024. [Online.] Available:
[https://en.cppreference.com/w/cpp/language/coroutines](https://en.cppreference.com/w/cpp/language/coroutines)

[2] L. Baker, "Asymmetric Transfer," lewissbaker.github.io, Accessed: Apr. 19, 2024. [Online]. Available: [https://lewissbaker.github.io](https://lewissbaker.github.io)