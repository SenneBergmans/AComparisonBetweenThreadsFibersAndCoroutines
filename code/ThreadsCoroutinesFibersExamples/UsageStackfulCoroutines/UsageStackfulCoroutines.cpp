#include "StackfulCoroutinesBasics.h"
#include "StackfulCoroutinesProducerConsumer.h"

#include <iostream>

int main()
{
    StackfulCoroutinesBasics stackfulCoroutinesBasics = StackfulCoroutinesBasics();
    stackfulCoroutinesBasics.staticPushExample();
    std::cout << std::endl << std::endl;
    stackfulCoroutinesBasics.staticPullExample();
    std::cout << std::endl << std::endl;
    stackfulCoroutinesBasics.memberExample();
    std::cout << std::endl << std::endl;
    stackfulCoroutinesBasics.parameterExample();
    std::cout << std::endl << std::endl;
    stackfulCoroutinesBasics.nestedYieldExample();
    
    std::cout << std::endl;

    StackfulCoroutinesProducerConsumer stackfulCoroutinesProducerConsumer = StackfulCoroutinesProducerConsumer();
    stackfulCoroutinesProducerConsumer.runSingle();
    std::cout << std::endl << std::endl;
    stackfulCoroutinesProducerConsumer.runOrderedMulti();
}