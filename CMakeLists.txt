project(cpp-logger)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(${PROJECT_NAME} SHARED include/cpp-logger/logger.h src/cpp-logger/logger.cpp)