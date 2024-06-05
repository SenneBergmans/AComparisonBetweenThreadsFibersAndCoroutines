#include "StackfulCoroutinesProducerConsumer.h"

#include <iostream>
#include <vector>

void StackfulCoroutinesProducerConsumer::runSingle()
{
    std::vector<int> numberVector = std::vector<int>();
    int producedConsumedAmount = 10;

    boost::coroutines2::coroutine<void>::push_type producerCoroutine(std::bind(StackfulCoroutinesProducerConsumer::producerFunction, std::placeholders::_1, &numberVector, producedConsumedAmount, 0));
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

void StackfulCoroutinesProducerConsumer::producerFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::vector<int>* numberVector, int producedConsumedAmount, int index)
{
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

void StackfulCoroutinesProducerConsumer::consumerFunction(boost::coroutines2::coroutine<void>::pull_type& yield, std::vector<int>* numberVector, int producedConsumedAmount, int index)
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
            yield();
        }
        //Alternative placement of the yield statement causing a yield after every item consumed.
        //yield();
    }
}

void StackfulCoroutinesProducerConsumer::runOrderedMulti() {
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