# Ylog - C++ Logging Library
Ylog is a versatile C++ logging library designed to simplify and enhance the logging experience in your C++ applications. It provides various logging features, including different log levels, file-based logging, log rotation, and custom log message formatting.
## Features
- **Multiple Log Levels**: Ylog supports different log levels, including TRACE, DEBUG, INFO, WARNING, ERROR, CRITICAL, and FATAL, allowing you to categorize log messages based on their importance.
- **File-Based Logging**: You can log messages to files, making it easy to store and manage log data. Ylog provides options for specifying the log file path and configuring log files.
- **Log Rotation**: The library offers log rotation functionality, allowing you to limit the size of log files. When a log file exceeds a specified size, it automatically rotates the log, keeping your log files manageable.
- **Custom Log Message Formatting**: Ylog enables you to define custom log message formats, giving you full control over how log entries are structured.
- **Thread-Safe**: Ylog is designed with thread safety in mind, ensuring that log messages from multiple threads are handled correctly.
## Usage Examples
  ```cpp
  #include "Ylog_Logger_Predefs.h"
  
  // Create a ConsoleLogger instance with the desired log level (e.g., Ylog::Enums::Levels::TRACE).
  Ylog::ConsoleLogger cLogger(Ylog::Enums::Levels::TRACE);
  
  // Log a message with a specific log level and message text.
  cLogger.Log(Ylog::Enums::Levels::DEBUG, "This is a debug message.");
  
  // Log messages at different log levels (e.g., INFO, WARNING, ERROR).
  cLogger.Log(Ylog::Enums::Levels::INFO, "Informational message.");
  cLogger.Log(Ylog::Enums::Levels::WARNING, "Warning message.");
  cLogger.Log(Ylog::Enums::Levels::ERROR, "Error message.");
  ```
  ```cpp
  #include "Ylog_Logger_Predefs.h"

  // Create a FileLogger instance with the desired log level and log file path.
  Ylog::FileLogger fLogger("log_file.txt", Ylog::Enums::Levels::TRACE);
  
  // Log a message with a specific log level and message text.
  fLogger.Log(Ylog::Enums::Levels::INFO, "This is an informational message.");
  
  // Configure and rotate log files if needed.
  fLogger.Configure("new_log_file.txt", Ylog::Enums::Levels::DEBUG, "%Y-%m-%d %H:%M:%S");
  ```
  ```cpp
  #include "Ylog_Logger_Predefs.h"

  // Create an RFileLogger instance with the desired log level and log file path.
  Ylog::RFileLogger rfLogger("log_file.txt", Ylog::Enums::Levels::TRACE);
  
  // Enable log rotation and set rotation size (optional).
  rfLogger.Set(Ylog::Interfaces::IRFLogger::Modes::ROTATE, true);
  rfLogger.SetRotationSize(1024 * 1024); // Set rotation size to 1 MB
  
  // Log messages with log rotation.
  rfLogger.Log(Ylog::Enums::Levels::INFO, "This is an informational message.");
  
  // Rotate log files automatically when the size exceeds the specified limit.
  ```
  ```cpp
  #include "Ylog_Logger_Predefs.h"

  // Create a CRFLogger instance with the desired log level and log file path.
  Ylog::CRFLogger cfLogger("log_file.txt", Ylog::Enums::Levels::TRACE);
  
  // Enable log rotation and set rotation size (optional).
  cfLogger.Set(Ylog::Interfaces::IRFLogger::Modes::ROTATE, true);
  cfLogger.SetRotationSize(1024 * 1024); // Set rotation size to 1 MB
  
  // Set a custom log message format (optional).
  cfLogger.SetLogFormat("[{0}] - ({1}) - {2} : {3}");
  
  // Log messages with custom formatting and log rotation.
  cfLogger.Log(Ylog::Enums::Levels::INFO, "This is an informational message.");
  
  // Rotate log files automatically when the size exceeds the specified limit.
  ```
