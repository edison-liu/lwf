
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//#include <time.h>

#include <logging.h>

using namespace LWF;

namespace LWF
{

LogStream::LogStream()
{
    memset(text_, 0, sizeof(text_));
    num_of_char_ = 0;
}

LogStream::~LogStream()
{
    
}

const char* LogStream::str()
{
    return text_;
}

LogStream& LogStream::operator<<(const char *msg)
{
    //num_of_char_ += strncpy(static_cast<char *>(&text_[0] + num_of_char_), msg, sizeof(text_));
    num_of_char_ += sprintf(&text_[num_of_char_], "%s", msg);
}

LogStream& LogStream::operator<<(const std::string msg)
{
    //num_of_char_ += strncpy(static_cast<char *>(&text_[0] + num_of_char_), msg, sizeof(text_));
    num_of_char_ += sprintf(&text_[num_of_char_], "%s", msg.c_str());
}

LogStream& LogStream::operator<<(int i)
{
    num_of_char_ += sprintf(&text_[num_of_char_], "%d", i);
}

LogMessage::LogMessage(const char *file, int line, LogLevel l)
    : level_(l)
{
    Init(file, line);
}

void LogMessage::Init(const char *file, int line)
{
    localtime(&timestamp_);
}

LogMessage::~LogMessage()
{
    Flush();
}

void LogMessage::Flush()
{
    //printf("%s %s\r\n", ctime(&timestamp_), stream().str());
    Logger::Write(level_, timestamp_, stream().str(), strlen(stream().str()));
}

LogStream& LogMessage::stream()
{
    return stream_;
}


Logger* Logger::instance_ = 0;
LogLevel Logger::enabled_[LOG_LEVEL_MAX];
    
FILE* Logger::fp_ = 0;
bool Logger::is_file_open_ = false;
std::string Logger::filename_;


Logger* Logger::Instance()
{
    if (!instance_)
        instance_ = new Logger();
    
    return instance_;
}

int Logger::Write(LogLevel l, time_t timestamp, const char* msg, int len)
{
    int n = 0;
    Logger *logger = Instance();
    
    printf("%s", msg);
    
    if (!is_file_open_)
        CreateLogFile();
    
    if (fp_)
    {
        n = fwrite(msg, 1, len, fp_);
        fflush(fp_);
        
    }
    
    return n;
}

bool Logger::CreateLogFile()
{
    fp_ = fopen("test.txt", "aw+");
    
    is_file_open_ = true;
    return true;
}



}
