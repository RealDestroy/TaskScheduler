//
// Created by destroy on 2/7/24.
//
#pragma once
#ifndef TASKSCHEDULER_TASKS_H
#define TASKSCHEDULER_TASKS_H
#define PROJECT_NAME "TaskScheduler"

#endif //TASKSCHEDULER_TASKS_H
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include "DateUtility.h"



enum Command_Type {
    Toggle,ON,OFF
};
class Command {
public:
    Command(Command_Type type,int device_id);
    Command();

    int getDeviceId() const;
    Command_Type getCommandType();
    int getCommandTypeInt();
    static Command_Type getCommandTypeOf(int command_type_int);
    void sendToMQTT() const;
private:
    int device_id;
    Command_Type commandType;
};

class ExecutableTask {
public:
    ExecutableTask(TimeInfo& execution_time,Command& command);
    ExecutableTask(unsigned int id, TimeInfo& execution_time,Command& command);
    ExecutableTask(TimeInfo& execution_time,unsigned long long interval,Command& command);
    ExecutableTask(unsigned int id, TimeInfo& execution_time,unsigned long long interval,Command& command);
    unsigned int getId() const;
    bool isRecurring() const;
    unsigned long getInterval() const;
    void cancel();
    unsigned long long getStartTime() const;
    void execute();
    void log();
    bool isCancelled() const;
    bool isComplete() const;
    Command* getCommand() const;
    int getTimeZone() const;
    bool canExecute() const;
    bool isPastExecution() const;
    std::string string() const;
private:
    unsigned long id;
    Command* command;
    TimeInfo* execution_time;
    unsigned long interval{};
    bool complete = false;
    bool cancelled = false;
};










