#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <cstdlib>
#include <syslog.h>
#include <mutex>

class Logger
{
    bool use_syslog;
    FILE *f = nullptr;
    mutable std::mutex mtx;
    
public:
    Logger();
    ~Logger();
    void init(const char *filename);
    void init(bool use_syslog = true);
    void info(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
    void warning(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
    void error(const char *format, ...) __attribute__ ((format (printf, 2, 3)));
    void perror(const char *msg);
};

extern Logger logger;
extern bool debug_mode;

#endif