#pragma once

#include <vector>
#include <string>
#include <chrono>

class TimeUsage
{
public:
	TimeUsage() {}

	void runTests(std::string testName);

	//https://isocpp.org/wiki/faq/templates#separate-template-class-defn-from-decl
	template<typename F>
	static std::string testSoloFunction(F&& functionPointer) {
		const auto functionStart{ std::chrono::steady_clock::now() };
		functionPointer();
		const auto functionEnd{ std::chrono::steady_clock::now() };
		const std::chrono::duration<double> elapsedTime{ functionEnd - functionStart };
		return std::to_string(elapsedTime.count());
	}
	
	static bool showDebugPrint(int input);

protected:
	virtual std::vector<std::vector<std::string>> testFunctions(std::vector<std::string> testInformation) = 0;

private:
	static const bool _debugPrint = true;
	static const int _debugPrintThreshold = 16;
	static const std::vector<int> _testingAmounts;

	std::vector<std::string> addTestInformation();
};

