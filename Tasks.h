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
#include <ctime>
#include <thread>
#include <vector>


/**
 *
 * @param epochTime
 * @return a formatted time string
 */
std::string readableTime(long epochTime);
/**
 *
 * @param year
 * @param month
 * @param day
 * @param hour
 * @param minute
 * @param second
 * @return return a modified epoch time in CST
 */
long getTime(int year, int month, int day, int hour, int minute, int second);
/**
 *
 * @param year
 * @param month
 * @param day
 * @param hour
 * @param minute
 * @param second
 * @return return a modified epoch time in CST
 */
long getTime(int day, int hour, int minute, int second);
/**
 *
 * @param year
 * @param month
 * @param day
 * @param hour
 * @param minute
 * @param second
 * @return return the current epoch time in CST
 */
long getCurrentTime();
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
    ExecutableTask(long start_time,Command& command);
    ExecutableTask(long start_time,long interval,Command& command);
    int getId() const;
    bool isRecurring() const;
    long getInterval() const;
    void cancel();
    long getStartTime() const;
    void execute();
    bool isCancelled() const;
    bool isComplete() const;
    Command& getCommand();
private:
    int id;
    Command command;
    long first_execution;
    long interval{};
    bool complete = false;
    bool cancelled = false;
    bool recurring = false;
};








