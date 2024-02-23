//
// Created by Destroy on 2/12/2024.
//

#ifndef TASKSCHEDULER_JSONUTILITY_H
#define TASKSCHEDULER_JSONUTILITY_H

#endif //TASKSCHEDULER_JSONUTILITY_H

#include <iostream>
#include <sstream>
#include <vector>


namespace SchedulerUtil {
    std::vector<std::string> split(const std::string& s, const std::string& delimiter);
    bool contains(const std::vector<unsigned int>& vec, unsigned int value);
    const char* toChar(int i);
    const char* toChar(long i);
    const char* toChar(unsigned int i);
    const char* toChar(unsigned long i);
}

