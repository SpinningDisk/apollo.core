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
    set(CMAKE_C_FLAGS_DEBUG "-fsanitize=address -g -O0")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "-O3")
endif()
include_directories(${CMAKE_SOURCE_DIR}/../build/include)
link_directories(${CMAKE_SOURCE_DIR}/../build/lib)
#end copy paste

# deps
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# protocol
add_library(socket STATIC src/socket.c)
add_library(socket.shared SHARED src/socket.c)

target_include_directories(socket PUBLIC ${Python3_INCLUDE_DIRS})
target_include_directories(socket.shared PUBLIC ${Python3_INCLUDE_DIRS})

add_library(proto STATIC src/proto.c)
add_library(proto.shared SHARED src/proto.c)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries(proto.shared PUBLIC socket.shared)
    target_link_libraries(proto.shared PUBLIC asan)
    target_compile_options(proto PUBLIC -fsanitize=address -fno-omit-frame-pointer -g -O0)
    target_link_options(proto PUBLIC -fsanitize=address)
endif()

target_include_directories(proto PUBLIC ${Python3_INCLUDE_DIRS})
target_include_directories(proto PUBLIC include/)
target_include_directories(proto.shared PUBLIC ${Python3_INCLUDE_DIRS})
target_include_directories(proto.shared PUBLIC include/)


set_target_properties(socket PROPERTIES OUTPUT_NAME socket)
set_target_properties(socket.shared PROPERTIES OUTPUT_NAME socket)
set_target_properties(proto PROPERTIES OUTPUT_NAME proto)
set_target_properties(proto.shared PROPERTIES OUTPUT_NAME proto)
