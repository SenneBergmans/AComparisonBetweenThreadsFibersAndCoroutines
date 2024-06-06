#pragma once

#include <string>
#include <vector>
#include <fstream>

class FileWriter
{
public:
	void writeToNewCSV(std::vector<std::vector<std::string>> data, std::string fileName);

private:
	const char _separatorCharacter = ';';
	const char _csvEscapeCharacter = '\"';
	const std::string _filePath = "C:\\Users\\user\\Downloads\\"; //Set the file path here
	const std::string _fileExtension = ".csv";
	void writeCSVFile(std::ofstream* file, std::vector<std::vector<std::string>> data);
	void writeCSVLine(std::ofstream* file, std::vector<std::string> lineData);
	std::string createUniqueFileName(std::string fileName);
	std::string removeFileExtension(std::string fileName);
};

