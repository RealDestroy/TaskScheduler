//
// Created by destroy on 2/10/24.
//


#include "TaskScheduler.h"


void TaskScheduler::loop() {
    while(!HALT_SCHEDULER) {
        for(auto& task :  TaskScheduler::tasks) {
            if(task->canExecute()) {
                task->execute();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); //sleep loop for 1 second
    }
}
void TaskScheduler::readInSchedules() {
    while(!HALT_SCHEDULER) {
        //read file for new schedules
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
void TaskScheduler::start() {
    MAIN_THREAD = std::thread(&TaskScheduler::loop,this);
    UTILITY_THREAD = std::thread(&TaskScheduler::readInSchedules,this);
    MAIN_THREAD.join(); //wait until thread ends (never unless stopped manually)
    UTILITY_THREAD.join(); //this ends at the same time as the MAIN_THREAD
}
void TaskScheduler::stop() {
    HALT_SCHEDULER = true;
}

void TaskScheduler::add(ExecutableTask& task) {
    std::lock_guard<std::mutex> lock(taskLock);
    tasks.push_back(&task);
}
