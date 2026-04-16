cmake_minimum_required(VERSION 3.13)  # CMake version check
project(
    usrmgmt
    VERSION 0.1.0.1
    LANGUAGES C
)               # Create project "simple_example"

# config
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)

# deps

# usrmgmt
add_executable(manager src/manager.c)
target_link_libraries(manager PRIVATE sqlite3)
target_link_libraries(manager PRIVATE m)