#include <cstdarg>
#include <ctime>
#include <cstring>
#include <errno.h>

#include "taskmaster.h"
#include "Logger.h"

Logger logger;
bool debug_mode = false;

static const char *get_time()
{
    static char buffer[100];

    memset(buffer, 0, 100);
    time_t t = time(nullptr);
    struct tm *ti = localtime(&t);
    strftime(buffer, 100, "%d.%m.%Y %H:%M:%S", ti);
    return buffer;
}

Logger::Logger()
{}

Logger::~Logger()
{
    if (use_syslog)
        closelog();
    if (f)
        fclose(f);
}

void Logger::init(const char *filename)
{
    f = fopen(filename, "a+");
    if (!f) {
        fprintf(stderr, "Can't open log file: %s\n", strerror(errno));
        fprintf(stderr, "Using syslog instead\n");
        init(true);
        return ;
    }
    setvbuf(f, nullptr, _IONBF, 0);
}
void Logger::init(bool use_syslog)
{
    this->use_syslog = use_syslog;
    if (use_syslog)
        openlog(PROGRAMNAME, 0, LOG_DAEMON);
}

void Logger::info(const char *format, ...)
{
    va_list ap;

    std::lock_guard<std::mutex> lck(mtx);
    if (use_syslog) {
        va_start(ap, format);
        vsyslog(LOG_INFO, format, ap);
        va_end(ap);
    }
    if (f) {
        va_start(ap, format);
        fprintf(f, "[%s] INFO: ", get_time());
        vfprintf(f, format, ap);
        fprintf(f, "\n");
        va_end(ap);
    }
    if (debug_mode) {
        va_start(ap, format);
        fprintf(stderr, "[%s] INFO: ", get_time());
        vfprintf(stderr, format, ap);
        fprintf(stderr, "\n");
        va_end(ap);
    }
}

void Logger::warning(const char *format, ...)
{
    va_list ap;

    std::lock_guard<std::mutex> lck(mtx);
    if (use_syslog) {
        va_start(ap, format);
        vsyslog(LOG_WARNING, format, ap);
        va_end(ap);
    }
    if (f) {
        va_start(ap, format);
        fprintf(f, "[%s] WARNING: ", get_time());
        vfprintf(f, format, ap);
        fprintf(f, "\n");
        va_end(ap);
    }
    if (debug_mode) {
        va_start(ap, format);
        fprintf(stderr, "[%s] WARNING: ", get_time());
        vfprintf(stderr, format, ap);
        fprintf(stderr, "\n");
        va_end(ap);
    }
}

void Logger::error(const char *format, ...)
{
    va_list ap;

    std::lock_guard<std::mutex> lck(mtx);
    if (use_syslog) {
        va_start(ap, format);
        vsyslog(LOG_ERR, format, ap);
        va_end(ap);
    }
    if (f) {
        va_start(ap, format);
        fprintf(f, "[%s] ERROR: ", get_time());
        vfprintf(f, format, ap);
        fprintf(f, "\n");
        va_end(ap);
    }
    if (debug_mode) {
        va_start(ap, format);
        fprintf(stderr, "[%s] ERROR: ", get_time());
        vfprintf(stderr, format, ap);
        fprintf(stderr, "\n");
        va_end(ap);
    }
}

void Logger::perror(const char *msg)
{
    error("%s: %s", msg, strerror(errno));
}

