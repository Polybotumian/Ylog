# Ylog (Work In Progress)

Ylog is a flexible and lightweight C++ logging framework designed to simplify logging operations in your C++ applications. It offers a range of features for logging messages with different levels, customizing log formats, managing log file rotation, and more. Ylog is easy to integrate into your C++ projects and provides error handling.

## Features

- **Log Levels**: Ylog supports various log levels, including TRACE, DEBUG, INFO, WARNING, ERROR, CRITICAL, and FATAL. You can set the desired log level to control the verbosity of your logs.

- **Flexible Log Formats**: Customize the format of log messages using placeholders for thread ID, timestamp, log level, and the log message itself. Create log formats that suit your specific needs.

- **Log File Rotation**: Rotate log files based on size, ensuring that your log files don't grow indefinitely. Ylog allows you to specify a maximum log file size for rotation.

- **Multithreading Support**: Ylog is designed to work in multithreaded environments, providing thread safety for log operations.

- **Datetime Formatting**: Choose from predefined datetime formats or define your own to timestamp your log entries.

## Getting Started

### Prerequisites

- C++17 or later
- A C++ compiler (e.g., GCC, Clang, MSVC)

### Usage

To use Ylog in your C++ project:

1. Include the Ylog header file (`Ylog.h`) in your project.

2. Initialize and configure the logger as needed in your application code. Here's a basic example:

   ```cpp
   #include "Ylog.h"

   int main() {
       // Initialize the logger
       Ylog::Logger logger("myapp", ".log", Ylog::Levels::INFO, Ylog::DatetimeFormats::Ymd_HMS);

       // Log a message
       logger.Write(Ylog::Levels::INFO, "This is an information message.");

       // Set a custom log format
       logger.SetLogFormat("{1} [{2}] {3}");

       // Log another message with a custom format
       logger.Write(Ylog::Levels::ERROR, "An error occurred!");

       return 0;
   }
   ```
   ```cpp
   #include "Ylog.h"
      int main() {
          Ylog::Logger logger("myapp", ".log", Ylog::Levels::DEBUG, Ylog::DatetimeFormats::Ymd_HMS);
          logger.Enable(Ylog::Logger::Modes::ROTATE);
          logger.SetMaxSize(1000000000);
          logger.SetLogFormat("[{0}], ({1}), {2}: {3}");
      
          for (std::uint16_t i = 0; i < 20000; i++) {
              if (i % 2 == 0) {
                  logger.SetLevel(Ylog::Levels::DEBUG);
              } else if (i % 3 == 0) {
                  logger.SetLevel(Ylog::Levels::INFO);
              } else {
                  logger.SetLevel(Ylog::Levels::ERROR);
              }
              std::string message = "This is a trial! - " + std::to_string(i);
              logger.Write(Ylog::Levels::DEBUG, message.c_str());
              logger.Write(Ylog::Levels::INFO, message.c_str());
              logger.Write(Ylog::Levels::ERROR, message.c_str());
          }
      
          return 0;
      }
   ```
   ```cpp
   #include "Ylog.h"
   
   int main() {
       Ylog::Logger logger("rotating_log", ".log", Ylog::Levels::DEBUG, Ylog::DatetimeFormats::Ymd_HMS);
   
       // Enable log file rotation
       logger.Enable(Ylog::Logger::Modes::ROTATE);
       logger.SetMaxSize(1000000); // Set the maximum file size for rotation
   
       for (std::uint16_t i = 0; i < 10000; i++) {
           logger.Write(Ylog::Levels::INFO, "This is a log message.");
       }
   
       return 0;
   }
   ```

