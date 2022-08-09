//
// Created by haridev on 2/14/22.
//

#ifndef MIMIR_LOGGER_H
#define MIMIR_LOGGER_H

#include <cstdarg>
#include <memory>
#include <unordered_map>

namespace mimir {
enum LoggerType { NO_LOG = 0, LOG_ERROR = 1, LOG_WARN = 2, LOG_INFO = 3 };

class Logger {
 private:
  static std::unordered_map<std::string, std::shared_ptr<Logger>> instance_map;
  std::string _app_name;

 public:
  LoggerType level;

  Logger(std::string app_name)
      : _app_name(app_name), level(LoggerType::LOG_ERROR) {}

  static std::shared_ptr<Logger> Instance(std::string app_name = "MIMIR") {
    auto iter = instance_map.find(app_name);
    std::shared_ptr<Logger> instance;
    if (iter == instance_map.end()) {
      instance = std::make_shared<Logger>(app_name);
      instance_map.emplace(app_name, instance);
    } else {
      instance = iter->second;
    }
    return instance;
  }

  void mimir::Logger::log(LoggerType type, char *string, ...) {
    va_list args;
    va_start(args, string);
    char buffer[256];
    int resu = vsprintf(buffer, string, args);
    switch (type) {
      case LoggerType::LOG_INFO: {
        if (level >= LoggerType::LOG_INFO)
          fprintf(stdout, "[%s INFO]: %s\n", _app_name.c_str(), buffer);
        break;
      }
      case LoggerType::LOG_WARN: {
        if (level >= LoggerType::LOG_WARN)
          fprintf(stdout, "[%s WARN]: %s\n", _app_name.c_str(), buffer);
        break;
      }
      case LoggerType::LOG_ERROR: {
        if (level >= LoggerType::LOG_ERROR)
          fprintf(stderr, "[%s ERROR]: %s\n", _app_name.c_str(), buffer);
        break;
      }
    }

    va_end(args);
  }
};

std::unordered_map<std::string, std::shared_ptr<mimir::Logger>>
    mimir::Logger::instance_map =
        std::unordered_map<std::string, std::shared_ptr<mimir::Logger>>();
}  // namespace mimir

#endif  // MIMIR_LOGGER_H
