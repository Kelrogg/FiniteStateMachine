cmake_minimum_required(VERSION 3.17.0)
project(FsmConverter VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE FsmConverter_src
    "headers/*.h"
    "headers/*.hpp"
    "src/*.cpp"
    "main.cpp")
  
add_executable(FsmConverter ${FsmConverter_src})

target_include_directories(FsmConverter PUBLIC headers)

target_precompile_headers(FsmConverter PUBLIC headers/pch.hpp)
