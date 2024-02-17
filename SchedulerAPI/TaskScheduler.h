//
// Created by destroy on 2/10/24.
//
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include "Tasks.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include "JSONUtility.h"
#include <algorithm>
#include "filesystem"

#ifndef TASKSCHEDULER_TASKSCHEDULER_H
#define TASKSCHEDULER_TASKSCHEDULER_H
#define PACKET_LENGTH 6
#define TASK_IDS "task_ids"


#endif //TASKSCHEDULER_TASKSCHEDULER_H

class TaskScheduler {
public:
    void add(ExecutableTask& task);
    void start();
    void stop();
    void save();
    static ExecutableTask parsePacket(std::string& packet);
    static std::string cwd();
    void readInSchedules();
private:
    void loop();
    void task_handler() const;
    std::mutex taskLock;
    std::vector<ExecutableTask*> tasks;
    std::vector<unsigned int> task_ids;
    bool HALT_SCHEDULER = false;
    std::thread MAIN_THREAD;
    std::thread UTILITY_THREAD;
protected:
    static std::string file();
};
