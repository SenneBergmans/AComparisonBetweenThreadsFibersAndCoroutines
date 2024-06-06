#pragma once

#include <string>
#include <vector>
#include <fstream>

class MergesortFileReader
{
public:
	static bool binaryIntFileExists(int totalAmount);
	static void readBinaryIntFile(std::vector<int>* vector, int totalAmount, int numberAmount, int offset);

private:
	static const std::string _filePath;
	static const std::string _csvFileExtension;
};

