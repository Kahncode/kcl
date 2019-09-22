@echo off
setlocal

set BUILD_DIR=%~dp0Build

if not exist %BUILD_DIR% (
	mkdir %BUILD_DIR%
	cd %BUILD_DIR%
	cmake -G "MinGW Makefiles" -D CMAKE_C_COMPILER=mingw32-gcc -D CMAKE_CXX_COMPILER=mingw32-g++ -D CMAKE_CXX_FLAGS="-mconsole -O3" ..
) else (
	cd %BUILD_DIR%
	cmake --debug-outputs ..
)

pause