//
// Created by destroy on 2/7/24.
//

#include "Tasks.h"
#include <iostream>



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

/**
 * Sends the command via MQTT to the ESP32 to trigger the light
 */
void Command::sendToMQTT() const {
    //more code
    std::cout << "Command sent via MQTT to device " << getDeviceId() << std::endl;
}

ExecutableTask::ExecutableTask(TimeInfo execution_time, Command& command) {
    this->command = command;
    this->id = getNextTaskID();
    this->recurring = true;
    this->execution_time = execution_time;
}
ExecutableTask::ExecutableTask(TimeInfo execution_time, long interval, Command& command) {
    this->command = command;
    this->id = getNextTaskID();
    this->recurring = true;
    this->interval = interval;
    this->execution_time = execution_time;
}
int ExecutableTask::getId() const {
    return this->id;
}
bool ExecutableTask::isRecurring() const {
    return this->recurring;
}
long ExecutableTask::getInterval() const {
    return this->interval;
}
void ExecutableTask::cancel() {
    std::cout << "Task " << this->getId() << " cancelled." << std::endl;
    this->cancelled = true;
}
long ExecutableTask::getStartTime() const {
    return this->execution_time.getEpoch();
}
void ExecutableTask::execute() {
    if(isCancelled())return;
    if(isComplete()) return;
    command.sendToMQTT();
    std::cout << "Task " << this->getId() << " executed successfully." << std::endl;
    if(isRecurring()) {
        execution_time.modify(interval);
        if (execution_time.getEpoch() < getCurrentTime(getTimeZone())) {
            while(execution_time.getEpoch() < getCurrentTime(getTimeZone())) {
                execution_time.modify(interval);
            }
        }
    } else {
        complete = true;
    }
}
int ExecutableTask::getTimeZone() const {
    return execution_time.getTimeZone();
}
bool ExecutableTask::isCancelled() const {
    return cancelled;
}
bool ExecutableTask::isComplete() const {
    return this->complete;
}
Command& ExecutableTask::getCommand() {
    return command;
}





