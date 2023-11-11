cmake_minimum_required(VERSION 3.0)
project(cpp-logger)
#------------------------------------------------------------------------------
# Version information
#------------------------------------------------------------------------------
set(CPP_LOGGER_VERSION_MAJOR "0")
set(CPP_LOGGER_VERSION_MINOR "0")
set(CPP_LOGGER_VERSION_PATCH "1")
set(CPP_LOGGER_PACKAGE ${PROJECT_NAME})
set(CPP_LOGGER_PACKAGE_NAME ${PROJECT_NAME})
set(CPP_LOGGER_PACKAGE_VERSION "${CPP_LOGGER_VERSION_MAJOR}.${CPP_LOGGER_VERSION_MINOR}.${CPP_LOGGER_VERSION_PATCH}")
set(CPP_LOGGER_PACKAGE_VERSION_MAJOR "${CPP_LOGGER_VERSION_MAJOR}.${CPP_LOGGER_VERSION_MINOR}")
set(CPP_LOGGER_PACKAGE_VERSION_MINOR "${CPP_LOGGER_VERSION_PATCH}")
set(CPP_LOGGER_PACKAGE_STRING "${CPP_LOGGER_PACKAGE_NAME} ${CPP_LOGGER_PACKAGE_VERSION}")
set(CPP_LOGGER_PACKAGE_TARNAME "${CPP_LOGGER_PACKAGE}")
if (DEFINED CMAKE_BUILD_DIR)
    set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIR})
endif()

set(CPP_LOGGER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp-logger/logger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp-logger/clogger.cpp)
set(CPP_LOGGER_PRIVATE_INCLUDE)
set(CPP_LOGGER_PUBLIC_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-logger/logger.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cpp-logger/clogger.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${CPP_LOGGER_SRC}
        )
target_include_directories(${PROJECT_NAME}
        PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )

#------------------------------------------------------------------------------
# Setup install and output Directories
#------------------------------------------------------------------------------
if (NOT CPP_LOGGER_INSTALL_BIN_DIR)
    set(CPP_LOGGER_INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
    message(STATUS ${CPP_LOGGER_INSTALL_BIN_DIR})
endif ()
if (NOT CPP_LOGGER_INSTALL_LIB_DIR)
    set(CPP_LOGGER_INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
endif ()
if (NOT CPP_LOGGER_INSTALL_INCLUDE_DIR)
    set(CPP_LOGGER_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
endif ()
if (NOT CPP_LOGGER_INSTALL_DATA_DIR)
    set(CPP_LOGGER_INSTALL_DATA_DIR ${CMAKE_INSTALL_PREFIX}/share)
endif ()

if (NOT CMAKE_INSTALL_RPATH)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

#------------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
        )

set(CMAKE_INCLUDE_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/include CACHE PATH "Store the headers."
        )
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/lib CACHE PATH "Single Directory for all Libraries"
        )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/lib CACHE PATH "Single Directory for all static libraries."
        )

set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
#------------------------------------------------------------------------------
# Disallow in-source build
#------------------------------------------------------------------------------
if ("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR
            "${PROJECT_NAME} requires an out of source Build. "
            "Please create a separate binary directory and run CMake there.")
endif ()

#------------------------------------------------------------------------------
# Set a default build type if none was specified
#------------------------------------------------------------------------------
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
            "MinSizeRel" "RelWithDebInfo")
endif ()

if (NOT "${CMAKE_CXX_STANDARD}")
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif ()

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects.
#-----------------------------------------------------------------------------
if (NOT CPP_LOGGER_EXPORTED_TARGETS)
    set(CPP_LOGGER_EXPORTED_TARGETS "${PROJECT_NAME}-targets")
endif ()

set(CPP_LOGGER_LIBTYPE SHARED)
set_target_properties(${libtarget}
        PROPERTIES
        DEBUG_OUTPUT_NAME ${LIB_DEBUG_NAME}
        RELEASE_OUTPUT_NAME ${LIB_RELEASE_NAME}
        MINSIZEREL_OUTPUT_NAME ${LIB_RELEASE_NAME}
        RELWITHDEBINFO_OUTPUT_NAME ${LIB_RELEASE_NAME}
        VERSION ${LIB_VERSION}
        SOVERSION ${LIB_VERSION}
        )

include(GNUInstallDirs)
# the variant with PUBLIC_HEADER property unfortunately does not preserve the folder structure
#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
# so instead we iterate through public headers and install them "manually"
foreach (header ${CPP_LOGGER_PUBLIC_INCLUDE})
    file(RELATIVE_PATH header_file_path "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}" "${header}")
    get_filename_component(header_directory_path "${header_file_path}" DIRECTORY)
    install(
            FILES ${header}
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${header_directory_path}"
    )
    file(COPY ${header}
            DESTINATION "${CMAKE_INCLUDE_OUTPUT_DIRECTORY}/${header_directory_path}")
endforeach ()
#-----------------------------------------------------------------------------
# Configure the config.cmake file for the build directory
#-----------------------------------------------------------------------------
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/CMake/${PROJECT_NAME}-config.cmake.build.in
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/${PROJECT_NAME}-config.cmake @ONLY
)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/CMake/${PROJECT_NAME}-config.cmake.install.in
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/install/${PROJECT_NAME}-config.cmake @ONLY
)
install(
        FILES
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/install/${PROJECT_NAME}-config.cmake
        DESTINATION
        ${CPP_LOGGER_INSTALL_LIB_DIR}/cmake/${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#-----------------------------------------------------------------------------
# Configure the ${PROJECT_NAME}-config-version .cmake file for the install directory
#-----------------------------------------------------------------------------
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/CMake/${PROJECT_NAME}-config-version.cmake.in
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/${PROJECT_NAME}-config-version.cmake @ONLY
)

install(
        FILES
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/${PROJECT_NAME}-config-version.cmake
        DESTINATION
        ${CPP_LOGGER_INSTALL_LIB_DIR}/cmake/${PROJECT_NAME}
)
install(
        TARGETS
        ${PROJECT_NAME}
        EXPORT
        ${CPP_LOGGER_EXPORTED_TARGETS}
        LIBRARY DESTINATION ${CPP_LOGGER_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${CPP_LOGGER_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${CPP_LOGGER_INSTALL_BIN_DIR}
)
export(EXPORT ${CPP_LOGGER_EXPORTED_TARGETS}
        FILE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cmake/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake")
install(EXPORT
        ${CPP_LOGGER_EXPORTED_TARGETS}
        DESTINATION
        ${CPP_LOGGER_INSTALL_LIB_DIR}/cmake/${PROJECT_NAME}
        FILE
        ${CPP_LOGGER_EXPORTED_TARGETS}.cmake
        )
