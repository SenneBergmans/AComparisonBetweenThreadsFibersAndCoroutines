#include "MergesortFileReader.h"

#include <iostream>
#include <chrono>
#include <format>

const std::string MergesortFileReader::_filePath = "C:\\Users\\user\\Documents\\TestFiles\\"; //Set the filePath here
const std::string MergesortFileReader::_csvFileExtension = ".bin";

bool MergesortFileReader::binaryIntFileExists(int totalAmount)
{
	return false;
}

void MergesortFileReader::readBinaryIntFile(std::vector<int>* vector, int totalAmount, int numberAmount, int offset)
{
	FILE* reader;

	std::string fullFilePath = _filePath + std::to_string(totalAmount) + _csvFileExtension;
	
	if (fopen_s(&reader, fullFilePath.c_str(), "rb") != 0) {
		return;
	}

	if (reader == 0) {
		return;
	}

	if (vector->size() < numberAmount) {
		vector->assign(numberAmount, 0);
	}

	std::fseek(reader, (offset * sizeof 1), SEEK_SET);
	std::fread(vector->data(), sizeof vector->at(0), vector->size(), reader);

	std::fclose(reader);
}
