cmake_minimum_required(VERSION 3.10)
project(solar-sym)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/system.cpp
)

# Include directories
include_directories(
    header
    /usr/include/eigen3
)

add_executable(solar-sym ${SOURCES})