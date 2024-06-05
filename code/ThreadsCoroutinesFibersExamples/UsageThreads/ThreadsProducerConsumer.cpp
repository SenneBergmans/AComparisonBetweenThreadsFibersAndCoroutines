#include "ThreadsProducerConsumer.h"

#include <iostream>

void ThreadsProducerConsumer::runExample()
{
    std::vector<int> numberVector = std::vector<int>();
    boost::mutex threadMutex;

    boost::thread producerThread(producerFunction, &numberVector, &threadMutex);
    boost::thread consumerThread(consumerFunction, &numberVector, &threadMutex);

    producerThread.join();
    consumerThread.join();
}

void ThreadsProducerConsumer::producerFunction(std::vector<int>* numberVector, boost::mutex* threadMutex)
{
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

void ThreadsProducerConsumer::consumerFunction(std::vector<int>* numberVector, boost::mutex* threadMutex)
{
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
