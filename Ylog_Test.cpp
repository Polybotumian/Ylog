#include "Ylog_Logger_Predefs.h"
#include <iostream>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	Ylog::FileLogger fLogger(
		(std::filesystem::current_path().string() + "\\FL_Test.log").c_str(),
		Ylog::Enums::Levels::TRACE
	);
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 20000; i++)
	{
		fLogger.Log(Ylog::Enums::Levels::DEBUG, "FileLogger_TEST_" + std::to_string(i));
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;

	Ylog::CRFLogger cfLogger(
		(std::filesystem::current_path().string() + "\\CRFL_Test.log").c_str(),
		Ylog::Enums::Levels::TRACE
	);
	cfLogger.Set(Ylog::Interfaces::IRFLogger::Modes::ROTATE, true);
	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 20000; i++)
	{
		cfLogger.Log(Ylog::Enums::Levels::DEBUG, "FileLogger_TEST_" + std::to_string(i));
	}
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;

	return 0;
}