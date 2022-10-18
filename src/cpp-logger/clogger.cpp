#include <cpp-logger/clogger.h>
#include <cpp-logger/logger.h>

void clog(int logger_level, const char *name, const char *string, ...) {
    va_list args;
    va_start(args, string);
    cpplogger::Logger::Instance(name)->log(static_cast<cpplogger::LoggerType>(logger_level),
                                           string, args);
    va_end(args);
}
