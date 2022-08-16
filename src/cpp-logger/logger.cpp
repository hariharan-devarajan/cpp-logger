//
// Created by hariharan on 8/16/22.
//
#include <cpp-logger/logger.h>

std::unordered_map<std::string, std::shared_ptr<cpplogger::Logger>>
    cpplogger::Logger::instance_map =
        std::unordered_map<std::string, std::shared_ptr<cpplogger::Logger>>();