#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include <filesystem>
#include <bitset>

namespace Ylog
{
	enum Levels		// Log level flags.
	{
		TRACE,		// Detailed trace information
		DEBUG,		// Debugging information
		VERBOSE,
		INFO,		// General information
		WARNING,	// Warnings
		ERROR,		// Errors
		CRITICAL,	// Critical errors
		FATAL,
		LEVEL_COUNT	// Total count of log level flags.
	};

	enum DatetimeFormats	// Predefined datetime formats.	
	{
		Ymd_HMS,			// Year.Month.day Hour:Minute:Second
		Ymd_HM,				// Year.Month.day Hour:Minute
		Ymd_H,				// Year.Month.day Hour
		Ymd,				// Year.Month.day
		Ym,					// Year.Month
		Year,				// Year
		FORMAT_COUNT		// Total count of predefined datetime formats.
	};

	enum MessageFormatTags
	{
		THREAD_ID = '0',
		TIMESTAMP = '1',
		LOG_LEVEL = '2',
		MESSAGE = '3',
		MESSAGE_FORMAT_COUNT
	};

	class Logger
	{
	public:
		enum Modes
		{
			ROTATE,
			MODE_COUNT
		};
	private:
		struct Log									// Data structure for log file.
		{
			Log();
			~Log();
			std::string name;						// Name of the log file.
			std::string extension;					// Extension of the log file.
			std::ofstream file;						
			std::filesystem::path path;
		};

		const char* _levels[Levels::LEVEL_COUNT];	// Log level strings to print.
		std::string _current_datetime_format;		// Self explanatory.
		Log _log;									// Log file.
		std::mutex _mutex;							// Thread lock.
		std::uint8_t _current_level;				// Current log level. Below of it will not be logged.
		std::uint32_t _max_size;					// In bytes.
		std::uint8_t _rotated_file_count;			// Self explanatory.
		std::bitset<Logger::Modes::MODE_COUNT> _modes;
		std::string _current_message_format;
		/// <summary>
		/// To get current datetime string.
		/// </summary>
		/// <param name="format"></param>
		/// <returns>Current datetime as std::string</returns>
		std::string GetTime(const char* format);
		/// <summary>
		/// To create seperate log files based on size. If size >= _max_size, new log file will be created.
		/// </summary>
		void Rotate();

	public:
		Logger(const char* file_name, const char* extension, std::uint8_t level, const char* _current_datetime_format);
		~Logger();
		/// <summary>
		/// Logs the message with flag.
		/// </summary>
		/// <param name="flag"> is an enum from Ylog::Levels </param>
		/// <param name="message"></param>
		void Write(std::uint8_t flag, const char* message);
		/// <summary>
		/// Sets current log level, Below of it will not be logged.
		/// </summary>
		/// <param name="new_level"> is an enum from Ylog::Levels </param>
		void SetLevel(std::uint8_t new_level);
		/// <summary>
		/// Sets current datetime format.
		/// </summary>
		/// <param name="new_format"></param>
		void SetDatetimeFormat(const char* new_format);
		/// <summary>
		/// Sets new size for log file rotation.
		/// </summary>
		/// <param name="new_size"></param>
		void SetMaxSize(std::uint32_t new_size);
		void SetLogFormat(const char* new_format);
		/// <summary>
		/// Reconfigures and re-opens current log file with provided arguments.
		/// </summary>
		/// <param name="file_name"></param>
		/// <param name="extension"></param>
		/// <param name="level"></param>
		/// <param name="current_datetime_format"></param>
		void Configure(const char* file_name, const char* extension, std::uint8_t level, const char* _current_datetime_format);
		/// <summary>
		/// Gets predefined datetime formats via Ylog::DatetimeFormats enum.
		/// </summary>
		/// <param name="flag"></param>
		/// <returns>Predefined datetime format string</returns>
		static const char* DatetimeFormat(std::uint8_t flag);
		void Enable(std::uint8_t mode);
		void Disable(std::uint8_t mode);
	};
}
