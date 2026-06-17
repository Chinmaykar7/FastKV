cmake_minimum_required(VERSION 3.16)

project(RedisLite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(redislite
    src/main.cpp
    src/server.cpp
)

target_include_directories(redislite
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(redislite
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
