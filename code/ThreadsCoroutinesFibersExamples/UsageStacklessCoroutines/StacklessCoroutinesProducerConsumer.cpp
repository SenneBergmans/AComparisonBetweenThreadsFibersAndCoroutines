#include "StacklessCoroutinesProducerConsumer.h"

#include <iostream>
#include <vector>

void StacklessCoroutinesProducerConsumer::runSingle()
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

CoroutineHandler<bool> StacklessCoroutinesProducerConsumer::producerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index)
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

CoroutineHandler<bool> StacklessCoroutinesProducerConsumer::consumerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index)
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

void StacklessCoroutinesProducerConsumer::runOrderedMulti() {
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