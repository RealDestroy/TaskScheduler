//
// Created by destroy on 2/10/24.
//
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include "Tasks.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include "SchedulerUtil.h"
#include <algorithm>
#include <list>
#include "filesystem"


#ifndef TASKSCHEDULER_TASKSCHEDULER_H
#define TASKSCHEDULER_TASKSCHEDULER_H
#define PACKET_LENGTH 6
#define TASK_IDS "task_ids"


#endif //TASKSCHEDULER_TASKSCHEDULER_H

class TaskScheduler {
public:
    bool add(ExecutableTask& task);
    void start();
    void stop();
    void save();
    static ExecutableTask parsePacket(std::string& packet);
    static std::string cwd();
    void retrieveLocalSchedules();
    bool retrieveCloudSchedules();
private:
    void loop();
    void task_handler();
    std::mutex taskLock;
    std::vector<ExecutableTask*> tasks;
    std::vector<unsigned int> task_ids;
    bool HALT_SCHEDULER = false;
    std::thread MAIN_THREAD;
    std::thread UTILITY_THREAD;
protected:
    static std::string file();
    static std::string local_file();
};
