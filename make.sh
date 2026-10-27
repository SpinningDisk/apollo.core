#!/bin/bash

base="/mnt/data/dev/apollo/.core"

lang=$(echo $1|tr '[:upper:]' '[:lower:]')
cmk=$(cat << 'EOF'
# copy paste section\n
cmake_minimum_required(VERSION 3.13)  # CMake version check\n
project(\n
    apollo-tests\n
    VERSION 0.1.0.1\n
    LANGUAGES C\n
)\n
\n
# config\n
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)\n
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)\n
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/bin)\n
\n
if(CMAKE_BUILD_TYPE STREQUAL "Debug")\n
    add_compile_definitions(DEBUG)\n
    set(CMAKE_C_FLAGS_DEBUG "-fsanitize=address -g -O0")\n
endif()\n
if(CMAKE_BUILD_TYPE STREQUAL "Release")\n
    set(CMAKE_C_FLAGS_RELEASE "-O3")\n
endif()\n
include_directories(${CMAKE_SOURCE_DIR}/../build/include)\n
link_directories(${CMAKE_SOURCE_DIR}/../build/lib)\n
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)\n
include_directories(tests PRIVATE ${Python3_INCLUDE_DIRS})\n
#end copy paste\n
EOF
)

mkdir -p $base/$2
case "$lang" in
    "c")
        mkdir -p $base/$2/src
        mkdir -p $base/$2/include
        mkdir -p $base/$2/build/bin
        mkdir -p $base/$2/build/lib
        mkdir -p $base/$2/build/include
        echo -e $cmk >> $base/$2/CMakeLists.txt
        ;;
    "cpp")
        mkdir -p $base/$2/src
        mkdir -p $base/$2/include
        mkdir -p $base/$2/build/bin
        mkdir -p $base/$2/build/lib
        mkdir -p $base/$2/build/include
        echo -e $cmk >> $base/$2/CMakeLists.txt
        ;;
    "java")
        mkdir -p $base/$2/src
        mkdir -p $base/$2/include
        mkdir -p $base/$2/build
        touch $base/$2/pom.xml
        ;;
    "python")
        touch $base/$2/setup.py
        touch $base/$2/__init__.py
        ;;
    "rust")
        cargo new $2
        ;;
    *)
        echo "Language not supported"
        exit 1
        ;;
esac

echo "Language $lang created"
