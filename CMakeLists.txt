cmake_minimum_required(VERSION 3.26)
project(TaskScheduler)

set(CMAKE_CXX_STANDARD 17)

add_executable(TaskScheduler main.cpp
        Tasks.cpp
        Tasks.h
        DateUtility.h)
