cmake_minimum_required(VERSION 4.0)
project(fitusb2)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.cpp) # Source Files
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS inc/*.hpp) # Header Files

add_executable(fitusb2 ${SOURCE_FILES} ${HEADER_FILES})

target_compile_options(fitusb2 PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(fitusb2 PRIVATE inc)