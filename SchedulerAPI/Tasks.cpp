//
// Created by destroy on 2/7/24.
//
#define _GLIBCXX_USE_NANOSLEEP
#include "Tasks.h"
#include <iostream>
#include <string>
#include <thread>



int lastTask = 0;
int getNextTaskID() {
    return lastTask ++;
}




Command::Command(Command_Type type, int device_id) {
    this->commandType = type;
    this->device_id = device_id;
}
Command_Type Command::getCommandType() {
    return commandType;
}
int Command::getDeviceId() const {
    return device_id;
}

Command::Command() {
    device_id = -1;
    commandType = Toggle;
}
Command_Type Command::getType(const std::string& cmd_type) {
    if(cmd_type == "Toggle") {
        return Toggle;
    }
    if(cmd_type == "OFF") {
        return OFF;
    }
    if(cmd_type == "ON") {
        return OFF;
    }
    return Toggle;
}

/**
 * Sends the command via MQTT to the ESP32 to trigger the light
 */
void Command::sendToMQTT() const {
    //more code
    std::cout << "Command sent via MQTT to device " << getDeviceId() << std::endl;
}
ExecutableTask::ExecutableTask(unsigned int id, TimeInfo &execution_time, unsigned long long int interval, Command &command) {
    this->command = &command;
    this->id = getNextTaskID();
    this->interval = interval;
    this->id = id;
    this->execution_time = &execution_time;
    //catch up to current time
    if (execution_time.getEpoch() < getCurrentTime(getTimeZone())) {
        while(execution_time.getEpoch() < getCurrentTime(getTimeZone())) {
            execution_time.modify(interval);
        }
    }
}

ExecutableTask::ExecutableTask(TimeInfo& execution_time, Command& command) {
    this->command = &command;
    this->id = getNextTaskID();
    this->execution_time = &execution_time;
}
ExecutableTask::ExecutableTask(TimeInfo& execution_time, unsigned long long interval, Command& command) {
    this->command = &command;
    this->id = getNextTaskID();
    this->interval = interval;
    this->execution_time = &execution_time;
    //catch up to current time
    if (execution_time.getEpoch() < getCurrentTime(getTimeZone())) {
        while(execution_time.getEpoch() < getCurrentTime(getTimeZone())) {
            execution_time.modify(interval);
        }
    }
}
unsigned int ExecutableTask::getId() const {
    return this->id;
}
bool ExecutableTask::isRecurring() const {
    return this->interval > 0;
}
unsigned long ExecutableTask::getInterval() const {
    return this->interval;
}
void ExecutableTask::cancel() {
    std::cout << "Task " << this->getId() << " cancelled." << std::endl;
    this->cancelled = true;
}
unsigned long long ExecutableTask::getStartTime() const {
    return this->execution_time->getEpoch();
}
void ExecutableTask::execute() {
    if(isCancelled())return;
    if(isComplete()) return;
    command->sendToMQTT();
    std::cout << "Task " << this->getId() << " executed successfully." << std::endl;
    if(isRecurring()) {
        execution_time->modify(interval);
        //catch up to current time
        if (execution_time->getEpoch() < getCurrentTime(getTimeZone())) {
            while(execution_time->getEpoch() < getCurrentTime(getTimeZone())) {
                execution_time->modify(interval);
            }
        }
    } else {
        complete = true;
    }
}
int ExecutableTask::getTimeZone() const {
    return execution_time->getTimeZone();
}
bool ExecutableTask::isCancelled() const {
    return cancelled;
}
bool ExecutableTask::isComplete() const {
    return this->complete;
}
Command* ExecutableTask::getCommand() const {
    return command;
}

bool ExecutableTask::canExecute() const {
    long time = getCurrentTime(getTimeZone());
    return time == getStartTime();
}

bool ExecutableTask::isPastExecution() const {
    long time = getCurrentTime(getTimeZone());
    return time > getStartTime();
}

void ExecutableTask::log() {
    //log successful executions and if a task is past execution time without having executed.
}

std::string ExecutableTask::string() const {
    //"id:time_zone:start_time:interval:command_type:remote_device"
    std::string s =
              std::to_string(getId()) + ":"
            + std::to_string(getTimeZone()) + ":"
            + std::to_string(getStartTime()) + ":"
            + std::to_string(getInterval()) + ":"
            + std::to_string(getCommand()->getCommandType()) + ":"
            + std::to_string(getCommand()->getDeviceId());
    return s;
}







//    auto target_time = std::chrono::system_clock::from_time_t(getStartTime());
//    std::this_thread::sleep_until(target_time);



