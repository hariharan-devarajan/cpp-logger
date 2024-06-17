#include <cpp-logger/clogger.h>
#include <cpp-logger/logger.h>

void cpp_logger_clog(int logger_level, const char *name, const char *string, ...) {
  va_list args;
  va_start(args, string);
  char buffer[4096];
  int result = vsprintf(buffer, string, args);
  cpplogger::Logger::Instance(name)->log(
      static_cast<cpplogger::LoggerType>(logger_level), buffer);
  va_end(args);
  (void) result;
}
void cpp_logger_clog_level(const int logger_level, const char *name) {
  cpplogger::Logger::Instance(name)->level = static_cast<cpplogger::LoggerType>(logger_level);
}
