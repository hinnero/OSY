cmake_minimum_required(VERSION 3.31)
project(Coloquium4)

set(CMAKE_CXX_STANDARD 20)

add_executable(Coloquium4 main.cpp
        sync_policy.hpp
        visitor.hpp
        filesystem_nodes.hpp
        tests.cpp)
