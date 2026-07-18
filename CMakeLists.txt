cmake_minimum_required(VERSION 4.1)

project(sCnake C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/bin")

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(sCnake ${SOURCES})

target_include_directories(sCnake PUBLIC include)