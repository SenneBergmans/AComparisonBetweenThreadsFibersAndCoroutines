#include "MergesortHelper.h"
#include "MergesortFileReader.h"
#include "MergesortFileWriter.h"
#include "MergesortParameters.h"

#include <iostream>

void MergesortHelper::sortVector(std::vector<int>* sortedVector, int totalAmount, int thisAmount, int offset) {
	MergesortFileReader::readBinaryIntFile(sortedVector, totalAmount, thisAmount, offset);

	if (sortedVector->size() != thisAmount) {
		std::cout << "Could not read file";
		return;
	}

	int toCheck = 1;
	while (toCheck < thisAmount) {
		int checking = toCheck;
		while (checking > 0 && sortedVector->at(checking) < sortedVector->at(checking - 1)) {
			std::swap(sortedVector->at(checking - 1), sortedVector->at(checking));
			checking -= 1;
		}
		toCheck += 1;
	}
}

void MergesortHelper::combineSortedVectors(std::vector<int>* sortedVector, std::vector<int>* leftSorted, std::vector<int>* rightSorted) {
	int leftCounter = 0;
	int rightCounter = 0;

	if (sortedVector->size() < (leftSorted->size() + rightSorted->size())) {
		sortedVector->assign((leftSorted->size() + rightSorted->size()), 0);
	}

	while (leftCounter < leftSorted->size() || rightCounter < rightSorted->size()) {
		if (leftCounter >= leftSorted->size()) {
			sortedVector->at(leftCounter + rightCounter) = rightSorted->at(rightCounter);
			rightCounter++;
		}
		else if (rightCounter >= rightSorted->size()) {
			sortedVector->at(leftCounter + rightCounter) = leftSorted->at(leftCounter);
			leftCounter++;
		}
		else if (leftSorted->at(leftCounter) < rightSorted->at(rightCounter)) {
			sortedVector->at(leftCounter + rightCounter) = leftSorted->at(leftCounter);
			leftCounter++;
		}
		else {
			sortedVector->at(leftCounter + rightCounter) = rightSorted->at(rightCounter);
			rightCounter++;
		}
	}
}

void MergesortHelper::createParameterFiles() {
	for (int totalAmount = MergesortParameters::totalAmountMin; totalAmount <= MergesortParameters::totalAmountMax; totalAmount += MergesortParameters::totalAmountStepSize) {
		MergesortFileWriter::createRandomNumberBinaryFile(totalAmount);
	}
}