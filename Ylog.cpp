#include "Ylog.h"

#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>

Ylog::Logger::Log::Log() : name{ "" }, extension{ "" }
{
}

Ylog::Logger::Log::~Log()
{
}

Ylog::Logger::Logger(const char* file_name, const char* extension, std::uint8_t level, const char* _current_datetime_format) :
    _levels{ "TRACE", "DEBUG", "VERBOSE", "INFO", "WARNING", "ERROR", "CRITICAL", "FATAL"},
    _current_datetime_format{ _current_datetime_format },
    _current_level{ level },
    _current_message_format{ "{0} | {1}, [{2}] - {3}" },
    _max_size { 100000 },
    _rotated_file_count { 0 }
{
    try
    {
        _log.name = file_name;
        _log.extension = extension;
        _log.path.assign(_log.name + _log.extension);
        _log.file.open(_log.name + _log.extension, std::ios::out | std::ios::trunc | std::ios::binary);
        if (!_log.file.is_open())
        {
            throw std::runtime_error("Ylog::Logger::Logger(): Failed to open \"" + _log.name + _log.extension + "\".");
        }
    }
    catch (const std::exception& ex)
    {
        if (!_log.file.is_open())
        {
            _log.file.open(_log.name + _log.extension, std::ios::out | std::ios::trunc | std::ios::binary);
        }
        Write(Levels::ERROR, ex.what());
        _log.file.close();
    }
}

Ylog::Logger::~Logger()
{
    try
    {
        _log.file.close();
        if (std::rename((_log.name + _log.extension).c_str(), (std::to_string(_rotated_file_count) + "_" + _log.name + "_" + _log.extension).c_str()) != 0)
        {
            throw std::runtime_error("Ylog::Logger::~Logger(): Failed to rename \"" + _log.name + _log.extension + "\".");
        }
    }
    catch (const std::exception& ex)
    {
        if (!_log.file.is_open())
        {
            _log.file.open(_log.name + _log.extension, std::ios::out | std::ios::trunc | std::ios::binary);
        }
        Write(Levels::ERROR, ex.what());
        _log.file.close();
    }
}

std::string Ylog::Logger::GetTime(const char* format)
{
    try
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        struct tm timeInfo;
        if (localtime_s(&timeInfo, &currentTime) == 0)
        {
            char timeStr[100];
            std::strftime(timeStr, sizeof(timeStr), format, &timeInfo);
            return timeStr;
        }
        else
        {
            throw std::runtime_error("Ylog::Logger::GetTime(): Failed to get curren time for \"" + _log.name + _log.extension + "\".");
        }
    }
    catch (const std::exception& ex)
    {
        if (!_log.file.is_open())
        {
            _log.file.open(_log.name + _log.extension, std::ios::out | std::ios::trunc | std::ios::binary);
        }
        Write(Levels::ERROR, ex.what());
        _log.file.close();
    }
}

void Ylog::Logger::Rotate()
{
    _log.file.flush();
    if (_modes[Modes::ROTATE] == 1 && std::filesystem::file_size(_log.path) >= _max_size)
    {
        try
        {
            std::cout << std::filesystem::file_size(_log.path) << std::endl;
            _log.file.close();
            if (std::rename((_log.name + _log.extension).c_str(), (std::to_string(_rotated_file_count) + "_" + _log.name + "_" + _log.extension).c_str()) != 0)
            {
                throw std::runtime_error("Ylog::Logger::Rotate(): Failed to rotate " + _log.name + _log.extension);
            }
            _rotated_file_count++;
            _log.file.open(_log.name + _log.extension, std::ios::out | std::ios::trunc | std::ios::binary);
        }
        catch (const std::exception& ex)
        {
            if (!_log.file.is_open())
            {
                _log.file.open(_log.name + _log.extension, std::ios::out | std::ios::trunc | std::ios::binary);
            }
            Write(Levels::ERROR, ex.what());
            _log.file.close();
        }
    }
}

void Ylog::Logger::Write(std::uint8_t flag, const char* message)
{
    if (flag >= _current_level)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        Rotate();
        std::ostringstream ss(_current_message_format);
        for (size_t i{ 0 }; i < _current_message_format.length(); i++)
        {
            if (_current_message_format[i] == '{')
            {
                i++;
                switch (_current_message_format[i])
                {
                case MessageFormatTags::THREAD_ID:
                    ss << std::this_thread::get_id();
                    break;
                case MessageFormatTags::TIMESTAMP:
                    ss << GetTime(_current_datetime_format.c_str());
                    break;
                case MessageFormatTags::LOG_LEVEL:
                    ss << _levels[flag];
                    break;
                case MessageFormatTags::MESSAGE:
                    ss << message;
                    break;
                }
            }
            else if (_current_message_format[i] != '}')
            {
                ss << _current_message_format[i];
            }
        }

        _log.file << ss.str() << '\n';
    }
}

void Ylog::Logger::SetLevel(std::uint8_t flag)
{
    _current_level = flag;
}

void Ylog::Logger::SetDatetimeFormat(const char* new_level)
{
    _current_datetime_format = new_level;
}

void Ylog::Logger::Configure(const char* file_name, const char* extension, std::uint8_t level, const char* _current_datetime_format)
{
    try
    {
        if (_log.file.is_open() == true)
        {
            _log.file.close();
        }
        if (std::rename((_log.name + _log.extension).c_str(), (std::string(file_name) + std::string(_log.extension)).c_str()))
        {
            throw std::runtime_error("Ylog::Logger::Configure(): Failed to rename " + _log.name + _log.extension + " with new configuration!");
        }

        _log.name = file_name;
        _log.extension = extension;
        _current_level = level;
        
        _log.file.open(_log.name + _log.extension, std::ios::out | std::ios::trunc | std::ios::binary);
        _log.path.assign(_log.name + _log.extension);

        if (!_log.file.is_open()) {
            throw std::runtime_error("Ylog::Logger::Configure(): Failed to reopen " + _log.name + _log.extension + " with new configuration!");
        }
    }
    catch (const std::exception& ex)
    {
        if (!_log.file.is_open())
        {
            _log.file.open(_log.name + _log.extension, std::ios::out | std::ios::trunc | std::ios::binary);
        }
        Write(Levels::ERROR, ex.what());
        _log.file.close();
    }
}

const char* Ylog::Logger::DatetimeFormat(std::uint8_t new_format)
{
    switch (new_format)
    {
    case DatetimeFormats::Ymd_HMS:
        return "%Y-%m-%d %H:%M:%S";
    case DatetimeFormats::Ymd_HM:
        return "%Y-%m-%d %H:%M";
    case DatetimeFormats::Ymd_H:
        return "%Y-%m-%d %H";
    case DatetimeFormats::Ymd:
        return "%Y-%m-%d";
    case DatetimeFormats::Ym:
        return "%Y-%m";
    case DatetimeFormats::Year:
        return "%Y";
    default:
        return "%Y-%m-%d %H:%M:%S";
    }
}

void Ylog::Logger::SetMaxSize(std::uint32_t new_size)
{
    _max_size = new_size;
}

void Ylog::Logger::SetLogFormat(const char* new_format)
{
    _current_message_format = new_format;
}

void Ylog::Logger::Enable(std::uint8_t mode)
{
    _modes.set(mode);
}

void Ylog::Logger::Disable(std::uint8_t mode)
{
    _modes.reset(mode);
}