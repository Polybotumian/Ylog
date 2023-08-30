#include "Ylog.h"
#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

    Ylog::Logger logger("Deneme", ".log", Ylog::Levels::DEBUG, Ylog::Logger::DatetimeFormat(Ylog::DatetimeFormats::Ymd_HMS));
	logger.Enable(Ylog::Logger::Modes::ROTATE);
	logger.SetMaxSize(1000000000);
	logger.SetLogFormat("[{0}], ({1}), {2}: {3}");

	auto start = std::chrono::high_resolution_clock::now();
	for (std::uint16_t i = 0; i < 20000; i++)
	{
		if (i % 2 == 0)
		{
			logger.SetLevel(Ylog::Levels::DEBUG);
		}
		else if (i % 3 == 0)
		{
			logger.SetLevel(Ylog::Levels::INFO);
		}
		else
		{
			logger.SetLevel(Ylog::Levels::ERROR);
		}
		std::string message = "This is a trial! This is a trial! This is a trial! This is a trial! This is a trial! This is a trial! This is a trial! - " + std::to_string(i);
		logger.Write(Ylog::DEBUG, message.c_str());
		logger.Write(Ylog::INFO, message.c_str());
		logger.Write(Ylog::ERROR, message.c_str());
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;

	std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;

    return 0;
}