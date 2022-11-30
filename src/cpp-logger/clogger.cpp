#include <cpp-logger/clogger.h>
#include <cpp-logger/logger.h>

void cpp_logger_clog(int logger_level, const char *name, const char *string, ...) {
  va_list args;
  va_start(args, string);
  cpplogger::Logger::Instance(name)->log(
      static_cast<cpplogger::LoggerType>(logger_level), string, args);
  va_end(args);
}
void cpp_logger_clog_level(const int logger_level, const char *name) {
  cpplogger::Logger::Instance(name)->level = static_cast<cpplogger::LoggerType>(logger_level);
}
