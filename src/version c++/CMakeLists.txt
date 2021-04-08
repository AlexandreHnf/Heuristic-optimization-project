cmake_minimum_required(VERSION 3.15)
project(version_c__)

set(CMAKE_CXX_STANDARD 14)

add_executable(version_c__ main.cpp flowshop.cpp pfspinstance.cpp pfspinstance.h flowshop.h)