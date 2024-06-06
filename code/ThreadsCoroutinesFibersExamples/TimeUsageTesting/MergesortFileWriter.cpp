#include "MergesortFileWriter.h"

#include <iostream>

const std::string MergesortFileWriter::_filePath = "C:\\Users\\user\\Documents\\TestFiles\\"; //Set the filePath here
const std::string MergesortFileWriter::_binFileExtension = ".bin";

void MergesortFileWriter::createRandomNumberBinaryFile(int totalAmount) {
	FILE* writer;

	std::string fullFilePath = _filePath + std::to_string(totalAmount) + _binFileExtension;

	if (fopen_s(&writer,fullFilePath.c_str(), "wb") != 0) {
		return;
	}

	for (int i = 0; i < totalAmount; i++) {
		int random = rand();
		auto retVal = std::fwrite(&random, sizeof random, 1, writer);
	}

	std::fclose(writer);
}