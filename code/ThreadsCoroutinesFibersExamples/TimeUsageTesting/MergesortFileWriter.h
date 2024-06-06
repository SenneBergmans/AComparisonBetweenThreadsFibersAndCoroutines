#pragma once

#include <string>

class MergesortFileWriter
{
public:
	static void createRandomNumberBinaryFile(int totalAmount);

private:
	static const std::string _filePath;
	static const std::string _binFileExtension;
};

