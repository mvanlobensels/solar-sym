cmake_minimum_required(VERSION 3.10)
project(solar-sym)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

pybind11_add_module(solar_sym src/bindings.cpp src/system.cpp)
target_include_directories(solar_sym PRIVATE header)

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