cmake_minimum_required(VERSION 3.17)
project(newton)

set(CMAKE_CXX_STANDARD 14)

add_executable(newton main.cpp exprtk.hpp FunctionMatrix.h)