# copy paste section
cmake_minimum_required(VERSION 3.13)  # CMake version check
project(
    apollo-kernel
    VERSION 0.1.0.1
    LANGUAGES C
)

# config
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
    set(CMAKE_C_FLAGS_DEBUG "-fsanitize=address -g -O0")
endif()
set(CMAKE_C_FLAGS "-O0")
include_directories(${CMAKE_SOURCE_DIR}/../build/include)
link_directories(${CMAKE_SOURCE_DIR}/../build/lib)
#end copy paste

add_library(kernel STATIC src/kernel.c)
target_link_libraries(kernel socket)
target_link_libraries(kernel proto)
