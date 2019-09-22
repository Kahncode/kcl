cmake_minimum_required(VERSION 3.10)
project(KCL)

file(GLOB_RECURSE SOURCES "Source/*")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCE_CODE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "Source/*")

foreach(SOURCE IN LISTS SOURCE_CODE)
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    string(REPLACE "/" "\\" SOURCE_PATH_MSVC "${SOURCE_PATH}")
    source_group("${SOURCE_PATH_MSVC}" FILES "${SOURCE}")
endforeach()

add_executable(KCL WIN32 ${SOURCES})

target_include_directories(KCL PRIVATE Source/)

set_property(TARGET KCL PROPERTY CXX_STANDARD 17)
set_target_properties(KCL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/Binaries)
set_target_properties(KCL PROPERTIES LINKER_LANGUAGE CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

	set_target_properties(KCL PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)

endif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

