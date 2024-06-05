#include "FibersBasics.h"
#include "FibersProducerConsumer.h"

#include <iostream>

int main()
{
    FibersBasics fibersBasics = FibersBasics();
    fibersBasics.staticExample();
    fibersBasics.memberExample();
    fibersBasics.parameterExample();
    fibersBasics.noYieldExample();
    fibersBasics.yieldExample();

    FibersProducerConsumer fibersProducerConsumer = FibersProducerConsumer();
    fibersProducerConsumer.runSingle();
    std::cout << std::endl << std::endl;
    fibersProducerConsumer.runUnorderedMulti();
    std::cout << std::endl << std::endl;
    fibersProducerConsumer.runOrderedMulti();
}