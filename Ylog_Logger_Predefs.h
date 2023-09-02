#pragma once

#include "Ylog_Logger_Interfaces.h"

namespace Ylog
{
	class FileLogger : public Ylog::Interfaces::IFileLogger
	{
	public:
		FileLogger(
			std::size_t buffer_size,
			const char* file_path,
			std::uint8_t logLevel,
			const char* timestamp_format = "%Y-%m-%d %H:%M:%S"
		);
		~FileLogger();
		void Log(std::uint8_t loglevel, std::string log_message) override;
	};
	class CustomizableFileLogger : public Ylog::Interfaces::ICRFLog
	{
	public:
		CustomizableFileLogger(
			std::size_t buffer_size,
			const char* file_path,
			std::uint8_t logLevel,
			const char* new_log_format = "[{0}] - ({1}) - {2} : {3}",
			const char* timestamp_format = "%Y-%m-%d %H:%M:%S"
		);
		~CustomizableFileLogger();
		void Log(std::uint8_t loglevel, std::string log_message) override;
	};
}