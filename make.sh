#!/bin/bash

base="/mnt/data/dev/apollo/.core"

lang=$(echo $1|tr '[:upper:]' '[:lower:]')


mkdir $base/$2
case "$lang" in
    "c")
        mkdir $base/$2/src
        mkdir $base/$2/include
        mkdir $base/$2/build
        touch $base/$2/CMakeLists.txt
        ;;
    "cpp")
        mkdir $base/$2/src
        mkdir $base/$2/include
        mkdir $base/$2/build
        touch $base/$2/CMakeLists.txt
        ;;
    "java")
        mkdir $base/$2/src
        mkdir $base/$2/include
        mkdir $base/$2/build
        touch $base/$2/pom.xml
        ;;
    "python")
        mkdir $base/$2/src
        mkdir $base/$2/include
        mkdir $base/$2/build
        touch $base/$2/setup.py
        ;;
    *)
        echo "Language not supported"
        exit 1
        ;;
esac

echo "Language $lang created"
