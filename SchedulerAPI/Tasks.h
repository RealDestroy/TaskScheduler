//
// Created by destroy on 2/7/24.
//
#pragma once
#ifndef TASKSCHEDULER_TASKS_H
#define TASKSCHEDULER_TASKS_H

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
    void sendToMQTT() const;
private:
    int device_id;
    Command_Type commandType;
};

class ExecutableTask {
public:
    ExecutableTask(TimeInfo& execution_time,Command& command);
    ExecutableTask(TimeInfo& execution_time,long interval,Command& command);
    int getId() const;
    bool isRecurring() const;
    long getInterval() const;
    void cancel();
    long getStartTime() const;
    void execute();
    void log();
    bool isCancelled() const;
    bool isComplete() const;
    Command& getCommand();
    int getTimeZone() const;
    bool canExecute() const;
    bool isPastExecution() const;
private:
    int id;
    Command command;
    TimeInfo* execution_time;
    long interval;
    bool complete = false;
    bool cancelled = false;
    bool recurring = false;
};










