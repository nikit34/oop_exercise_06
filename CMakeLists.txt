cmake_minimum_required(VERSION 3.5)

project(lab6)

add_executable(lab6 main.cpp)

set_property(TARGET lab6 PROPERTY CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
