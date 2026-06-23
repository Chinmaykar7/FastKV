cmake_minimum_required(VERSION 3.16)

project(FastKV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(fastkv
    src/main.cpp
    src/server.cpp
    src/resp_parser.cpp
)

target_include_directories(fastkv
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(fastkv
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
