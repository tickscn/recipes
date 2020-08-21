cmake_minimum_required(VERSION 2.6)
project(daina C CXX)
set(CMAKE_CXX_STANDARD 14)
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_PROJECT_NAME STREQUAL "daina")
    option(DAINA_BUILD_EXAMPLES "build example and test" ON)
endif()

if(CMAKE_BUILD_BITS EQUAL 32)
    set(CMAKE_CXX_FLAGS "$CMAKE_CXX_FLAGS" "-m32")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR})

string (TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(base)

