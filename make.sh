#!/bin/bash

base="/mnt/data/dv/apollo/.core"

lang=$(echo $1|tr '[:upper:]' '[:lower:]')

case $lang in
    "c")
        mkdir $base/$2/src
        mkdir $base/$2/include
        mkdir $base/$2/build
        touch $base/$2/CMakeLists.txt
        echo
