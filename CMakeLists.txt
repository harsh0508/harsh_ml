cmake_minimum_required(VERSION 3.15)
project(harsh_ml)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(harsh_ml
    src/main.cpp
    src/ml.cpp
)