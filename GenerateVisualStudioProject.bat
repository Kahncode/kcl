@echo off
setlocal

set BUILD_DIR=%~dp0Build

if not exist %BUILD_DIR% (
	mkdir %BUILD_DIR%
	cd %BUILD_DIR%
	cmake -G "Visual Studio 15 2017 Win64" ..
) else (
	cd %BUILD_DIR%
	cmake --debug-outputs ..
)

pause