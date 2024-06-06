#include "FileWriter.h"

#include <iostream>
#include <chrono>
#include <format>

void FileWriter::writeToNewCSV(std::vector<std::vector<std::string>> data, std::string fileName)
{
	std::ofstream csvFile;
	csvFile.open(createUniqueFileName(fileName));
	writeCSVFile(&csvFile, data);
	csvFile.close();
}

void FileWriter::writeCSVFile(std::ofstream* file, std::vector<std::vector<std::string>> data)
{
	for (std::vector<std::string> lineData : data) {
		writeCSVLine(file, lineData);
	}
}

void FileWriter::writeCSVLine(std::ofstream* file, std::vector<std::string> lineData)
{
	for (std::string cell : lineData) {
		*file << "\"" << cell << "\",";
	}
	*file << "\n";
}

std::string FileWriter::createUniqueFileName(std::string fileName)
{
	
	const std::chrono::time_point now{ std::chrono::system_clock::now() };
	std::string uniqueFileName = _filePath + removeFileExtension(fileName) + '_' + std::format("{:%d_%m_%Y_%H_%M_%OS}", now) + _fileExtension;
	std::cout << uniqueFileName;
	return uniqueFileName;
}

std::string FileWriter::removeFileExtension(std::string fileName)
{
	if (fileName.substr(fileName.length() - _fileExtension.length(), fileName.length()) == _fileExtension) {
		return fileName.substr(0, fileName.length() - _fileExtension.length());
	}
	else {
		return fileName;
	}
}
