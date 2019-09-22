@echo off
setlocal

cmake -G "MinGW Makefiles" -D CMAKE_C_COMPILER=mingw32-gcc -D CMAKE_CXX_COMPILER=mingw32-g++ -D CMAKE_CXX_FLAGS="-mconsole -O3" .

pause