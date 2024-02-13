//
// Created by Destroy on 2/12/2024.
//

#include "SchedulerAPI/JSONUtility.h"


std::vector<std::string> JSONUtility::split(const std::string& s, const std::string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> list;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        list.push_back (token);
    }

    list.push_back (s.substr (pos_start));
    return list;
}