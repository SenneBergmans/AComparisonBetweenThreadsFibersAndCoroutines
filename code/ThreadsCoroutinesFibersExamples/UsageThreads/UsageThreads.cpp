#include "ThreadsBasics.h"
#include "ThreadsProducerConsumer.h"
#include <iostream>

int main()
{
    ThreadsBasics threadsBasics = ThreadsBasics();
    threadsBasics.staticExample();
    std::cout << std::endl;
    threadsBasics.memberExample();
    std::cout << std::endl;
    threadsBasics.parameterExample();
    std::cout << std::endl;
    ThreadsProducerConsumer().runExample();
}