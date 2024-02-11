//
// Created by destroy on 2/10/24.
//
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include "Tasks.h"

#ifndef TASKSCHEDULER_TASKSCHEDULER_H
#define TASKSCHEDULER_TASKSCHEDULER_H

#endif //TASKSCHEDULER_TASKSCHEDULER_H

class TaskScheduler {
public:
    void add(ExecutableTask& task);
    void start();
    void stop();
private:
    void loop();
    void readInSchedules();
    std::mutex taskLock;
    std::vector<ExecutableTask*> tasks;
    bool HALT_SCHEDULER = false;
    std::thread MAIN_THREAD;
    std::thread UTILITY_THREAD;
};
