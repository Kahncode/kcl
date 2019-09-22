#!/bin/sh

mkdir -p Build
cd Build
cmake -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -D CMAKE_CXX_FLAGS="-O3" ..
