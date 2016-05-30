#ifndef _LWF_LOGGING_H_
#define _LWF_LOGGING_H_

#include <string>
#include <time.h>

namespace LWF
{

class Logger;

enum LogLevel
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_MAX,
};

class LogStream
{
public:
    LogStream();
    ~LogStream();
    
    LogStream& operator<<(const char *msg);
    LogStream& operator<<(int i);

    const char* str();

private:
    char text_[1024];
    int  num_of_char_;

};

#if 0
class LogMessageData
{};
#endif

class LogMessage
{
public:

    LogMessage(const char *file, int line, LogLevel l);
    ~LogMessage();
    
    void Init(const char *file, int line);
    void Flush();
    
    LogStream& stream();

private:
    LogStream   stream_;
    time_t      timestamp_;
    LogLevel    level_;

};

class Logger
{
public:
    Logger() {}
    ~Logger() {}
    //static void EnableLevel(LogLevel l);
    
    static int Write(LogLevel l, time_t timestamp, const char* msg, int len);

private:
    
    static Logger* Instance();
    static bool CreateLogFile();
    
    static Logger *instance_;
    static LogLevel enabled_[LOG_LEVEL_MAX];
    
    static FILE    *fp_;
    static bool is_file_open_;
    static std::string   filename_;
    
};

#define LOG_FATAL   \
    LWF::LogMessage(__FILE__, __LINE__, LWF::LOG_LEVEL_FATAL).stream()
    
#define LOG_ERROR   \
    LWF::LogMessage(__FILE__, __LINE__, LWF::LOG_LEVEL_ERROR).stream()
    
#define LOG_WARN   \
    LWF::LogMessage(__FILE__, __LINE__, LWF::LOG_LEVEL_WARN).stream()

#define LOG_INFO   \
    LWF::LogMessage(__FILE__, __LINE__, LWF::LOG_LEVEL_INFO).stream()

#define LOG_DEBUG   \
    LWF::LogMessage(__FILE__, __LINE__, LWF::LOG_LEVEL_DEBUG).stream()

#define LOG_TRACE   \
    LWF::LogMessage(__FILE__, __LINE__, LWF::LOG_LEVEL_TRACE).stream()
    
    
}

#endif // _LWF_LOG_H_
