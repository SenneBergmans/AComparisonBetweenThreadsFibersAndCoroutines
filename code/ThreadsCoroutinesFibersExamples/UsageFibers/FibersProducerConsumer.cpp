#include "FibersProducerConsumer.h"

#ifndef NOMINMAX //Reference [3] (https://stackoverflow.com/a/6884251)
# define NOMINMAX
#endif
#include <boost/fiber/all.hpp>
#include <iostream>
#include <vector>

void FibersProducerConsumer::runSingle()
{
    std::vector<int> numberVector = std::vector<int>();
    int producedConsumedAmount = 6;

    boost::fibers::fiber producerFiber(producerFunction, &numberVector, producedConsumedAmount, 0);
    boost::fibers::fiber consumerFiber(consumerFunction, &numberVector, producedConsumedAmount, 1);

    producerFiber.join();
    consumerFiber.join();
}

void FibersProducerConsumer::runUnorderedMulti()
{
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

void FibersProducerConsumer::runOrderedMulti()
{
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

void FibersProducerConsumer::producerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index)
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
            boost::this_fiber::yield();
        }
        //Alternative placement of the yield statement causing a yield after every item produced.
        //boost::this_fiber::yield();
    }
}

void FibersProducerConsumer::consumerFunction(std::vector<int>* numberVector, int producedConsumedAmount, int index)
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
            boost::this_fiber::yield();
        }
        //Alternative placement of the yield statement causing a yield after every item consumed.
        //boost::this_fiber::yield();
    }
}
