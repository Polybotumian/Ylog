#include "Ylog_Logger_Predefs.h"

Ylog::FileLogger::FileLogger(
    std::size_t buffer_size,
    const char* file_path,
    std::uint8_t logLevel,
    const char* timestamp_format
)
{
    try
    {
        _buffer.resize(buffer_size);
        _path = file_path;
        RemoveLogs(_path, _path.extension().string());
        _loglevel = logLevel;
        _timestampFormat = timestamp_format;
        _file.open(_path.filename().string(), std::ios::out | std::ios::trunc | std::ios::binary);
        if (!_file.is_open())
        {
            throw std::runtime_error("Ylog::FileLogger::FileLogger(): Failed to open \"" + _path.filename().string() + "\".");
        }
    }
    catch (const std::exception& ex)
    {
        if (!_file.is_open())
        {
            _file.open(_path.filename().string(), std::ios::out | std::ios::trunc | std::ios::binary);
        }
        std::string err_message = ex.what();
        Log(Ylog::Enums::Levels::ERROR, err_message);
        _file.close();
    }
};
Ylog::FileLogger::~FileLogger()
{

};
void Ylog::FileLogger::Log(std::uint8_t loglevel, std::string log_message)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (loglevel >= _loglevel)
    {
        _file << '[' << std::this_thread::get_id() 
            << "] - [" << GetTimestamp()
            <<"] - [" << _logLevels[_loglevel]
            <<"] : " << log_message << '\n';
    }
};
//************************************************************************************************
Ylog::CustomizableFileLogger::CustomizableFileLogger(
    std::size_t buffer_size,
    const char* file_path,
    std::uint8_t logLevel,
    const char* new_log_format,
    const char* timestamp_format
)
{
    try
    {
        _buffer.resize(buffer_size);
        _path = file_path;
        RemoveLogs(_path, _path.extension().string());
        _loglevel = logLevel;
        _log_format = new_log_format;
        _timestampFormat = timestamp_format;
        _file.open(_path.filename().string(), std::ios::out | std::ios::trunc | std::ios::binary);
        if (!_file.is_open())
        {
            throw std::runtime_error("Ylog::FileLogger::FileLogger(): Failed to open \"" + _path.filename().string() + "\".");
        }
    }
    catch (const std::exception& ex)
    {
        if (!_file.is_open())
        {
            _file.open(_path.filename().string(), std::ios::out | std::ios::trunc | std::ios::binary);
        }
        std::string err_message = ex.what();
        Log(Ylog::Enums::Levels::ERROR, err_message);
        _file.close();
    }
};
Ylog::CustomizableFileLogger::~CustomizableFileLogger()
{
    try
    {
        _file.close();
        if (std::rename((_path.filename().string()).c_str(), (std::to_string(_rotated_file_count) + "_" + _path.filename().string()).c_str()) != 0)
        {
            throw std::runtime_error("Ylog::Logger::~Logger(): Failed to rename \"" + _path.filename().string() + "\".");
        }
    }
    catch (const std::exception& ex)
    {
        _file.close();
        _file.open("FileLogger.log", std::ios::out | std::ios::trunc | std::ios::binary);
        std::string err_message = ex.what();
        Log(Ylog::Enums::Levels::ERROR, err_message);
        _file.close();
    }
};
void Ylog::CustomizableFileLogger::Log(std::uint8_t loglevel, std::string log_message)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (loglevel >= _loglevel)
    {
        Rotate();
        std::ostringstream ss(_log_format);
        FormatLog(ss, loglevel, log_message);
        _file << ss.str() << '\n';
    }
};
//************************************************************************************************