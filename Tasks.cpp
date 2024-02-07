//
// Created by destroy on 2/7/24.
//

#include "Tasks.h"
#include <iostream>

#define CST (-21600)

int lastTask = 0;
int getNextTaskID() {
    lastTask ++;
    return lastTask + 1;
}
std::string readableTime(long epochTime) {
    std::time_t time = epochTime;
    std::tm* tmPtr = std::gmtime(&time); // Convert to UTC time
    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
long getTime(int year, int month, int day, int hour, int minute, int second) {
    struct tm timeinfo{};
    timeinfo.tm_year = year - 1900; // Years since 1900
    timeinfo.tm_mon = month - 1;     // Month (0 - 11)
    timeinfo.tm_mday = day;          // Day of the month (1 - 31)
    timeinfo.tm_hour = hour;         // Hour (0 - 23)
    timeinfo.tm_min = minute;        // Minute (0 - 59)
    timeinfo.tm_sec = second;             // Second (0 - 59)
    timeinfo.tm_isdst = 0;           // Daylight Saving Time flag

    time_t local_time = mktime(&timeinfo); // Convert to local time

    return local_time + CST;
}
long getTime(int day, int hour, int minute, int second) {
    time_t now = time(nullptr); // Get the current time
    struct tm* localTime = localtime(&now); // Convert to local time
    return getTime(localTime->tm_year + 1900,localTime->tm_mon + 1, day,hour,minute,second);
}
long getCurrentTime() {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    // Extract the current year and month
    std::tm* currentDateTime = std::localtime(&currentTime);
    std::time_t desiredTime = std::mktime(currentDateTime);
    return desiredTime + CST;
}

ExecutableTask::ExecutableTask(long start_time) {
    this->id = getNextTaskID();
    this->first_execution = start_time;
}
ExecutableTask::ExecutableTask(long start_time,long interval) {
    this->id = getNextTaskID();
    this->recurring = true;
    this->interval = interval;
    this->first_execution = start_time;
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
    return this->first_execution;
}
void ExecutableTask::execute() {
    if(this->isCancelled()){return;}
    if(this->isComplete()){return;}
    std::cout << "Task " << this->getId() << " executed successfully." << std::endl;
    if(isRecurring()) {
        this->first_execution += this->interval;
    } else {
        this->complete = true;
    }
}
bool ExecutableTask::isCancelled() const {
    return this->cancelled;
}
bool ExecutableTask::isComplete() const {
    return this->complete;
}
