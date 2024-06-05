#include "StacklessCoroutinesBasics.h"
#include "StacklessCoroutinesProducerConsumer.h"

int main()
{
    StacklessCoroutinesBasics stacklessCoroutinesBasics = StacklessCoroutinesBasics();
    stacklessCoroutinesBasics.staticExample();
    std::cout << std::endl << std::endl;
    stacklessCoroutinesBasics.memberExample();
    std::cout << std::endl << std::endl;
    stacklessCoroutinesBasics.parameterExample();
    std::cout << std::endl << std::endl;

    StacklessCoroutinesProducerConsumer stacklessCoroutinesProducerConsumer = StacklessCoroutinesProducerConsumer();
    stacklessCoroutinesProducerConsumer.runSingle();
    std::cout << std::endl << std::endl;
    stacklessCoroutinesProducerConsumer.runOrderedMulti();
}