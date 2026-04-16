cmake_minimum_required(VERSION 3.13)  # CMake version check
project(
    proto
    VERSION 0.1.0.1
    LANGUAGES C
)               # Create project "simple_example"

# config
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)

# deps
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# protocol
add_library(socket STATIC src/socket.c)
target_include_directories(socket PUBLIC ${Python3_INCLUDE_DIRS})
add_library(proto STATIC src/proto.c)
target_include_directories(proto PUBLIC ${Python3_INCLUDE_DIRS})
target_include_directories(proto PUBLIC include/)

add_executable(test src/test.c)
target_include_directories(test PRIVATE include/)
target_link_libraries(test PRIVATE proto)
target_link_libraries(test PRIVATE socket)
target_link_libraries(test PRIVATE ${Python3_LIBRARIES})
target_link_libraries(test PRIVATE m)