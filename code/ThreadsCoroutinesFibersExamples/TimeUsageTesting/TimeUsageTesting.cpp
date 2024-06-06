#include "ContextSwitchTest.h"
#include "ProducerConsumerTest.h"
#include "MergesortTest.h"

#include <iostream>

int main()
{
	ContextSwitchTest contextSwitch = ContextSwitchTest();
	contextSwitch.runTests("ContextSwitch");

	ProducerConsumerTest producerConsumer = ProducerConsumerTest();
	producerConsumer.runTests("ProducerConsumer");

	MergesortTest mergesort = MergesortTest();
	mergesort.runTests("Mergesort");
}
