cmake_minimum_required(VERSION 3.14)
project(solar-sym C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# GLFW is not installed system-wide, so download and build it with the project.
include(FetchContent)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
    URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(glfw)

add_executable(solar-sym
    src/main.cpp
    src/system.cpp
    src/renderer.cpp
    src/galaxy.cpp
    src/solar_system.cpp
)

target_include_directories(solar-sym PRIVATE header)
target_link_libraries(solar-sym PRIVATE Eigen3::Eigen OpenGL::GL GLEW::GLEW glfw)
