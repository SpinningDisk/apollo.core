# copy paste section
cmake_minimum_required(VERSION 3.13)  # CMake version check
project(
    proto
    VERSION 0.1.0.1
    LANGUAGES C
)

# config
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()
set(CMAKE_C_FLAGS_DEBUG, "-fsanitize=address")
include_directories(${CMAKE_SOURCE_DIR}/../build/include)
link_directories(${CMAKE_SOURCE_DIR}/../build/lib)
#end copy paste

# deps
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# protocol
add_library(socket STATIC src/socket.c)
target_include_directories(socket PUBLIC ${Python3_INCLUDE_DIRS})
add_library(proto STATIC src/proto.c)
target_include_directories(proto PUBLIC ${Python3_INCLUDE_DIRS})
target_include_directories(proto PUBLIC include/)
