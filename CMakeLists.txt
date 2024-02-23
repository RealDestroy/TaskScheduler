cmake_minimum_required(VERSION 3.26)
project(TaskScheduler)

set(CMAKE_CXX_STANDARD 17)

add_executable(TaskScheduler main.cpp
        SchedulerAPI/Tasks.cpp
        SchedulerAPI/Tasks.h
        SchedulerAPI/DateUtility.h
        SchedulerAPI/DateUtility.cpp
        SchedulerAPI/TaskScheduler.cpp
        SchedulerAPI/TaskScheduler.h
        SchedulerAPI/SchedulerUtil.cpp
        SchedulerAPI/SchedulerUtil.h
)
