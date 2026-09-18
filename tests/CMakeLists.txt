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
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
include_directories(tests PRIVATE ${Python3_INCLUDE_DIRS})
#end copy paste

add_executable(tests src/tests.c)
target_link_libraries(tests PRIVATE proto)
target_link_libraries(tests PRIVATE socket)
