//
// Created by Destroy on 2/8/2024.
//

#include "DateUtility.h"

std::string readableTime(const long epochTime) {
    std::time_t time = epochTime;
    std::tm* tmPtr = std::gmtime(&time); // Convert to UTC time
    std::ostringstream oss;
    oss << std::put_time(tmPtr, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

long getCurrentTime(const int timezone) {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm* currentDateTime = std::localtime(&currentTime);
    std::time_t desiredTime = std::mktime(currentDateTime);
    return desiredTime + timezone;
}
TimeInfo::TimeInfo(int timezone) {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm* currentDateTime = std::localtime(&currentTime);
    std::time_t desiredTime = std::mktime(currentDateTime);

    this->epoch = desiredTime + timezone;
    this->timezone = timezone;
}
TimeInfo::TimeInfo() {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm* currentDateTime = std::localtime(&currentTime);
    std::time_t desiredTime = std::mktime(currentDateTime);

    this->epoch = desiredTime + timezone;
    this->timezone = CST;
}
TimeInfo::TimeInfo(const int day,const int hour,const int min,const int sec,const int timezone) {
    time_t now = time(nullptr); // Get the current time
    struct tm* localTime = localtime(&now); // Convert to local time
    struct tm timeinfo{};
    timeinfo.tm_year = localTime->tm_year;
    timeinfo.tm_mon = localTime->tm_mon;     // Month (0 - 11)
    timeinfo.tm_mday = day;          // Day of the month (1 - 31)
    timeinfo.tm_hour = hour;         // Hour (0 - 23)
    timeinfo.tm_min = min;        // Minute (0 - 59)
    timeinfo.tm_sec = sec;             // Second (0 - 59)
    timeinfo.tm_isdst = 0;           // Daylight Saving Time flag

    time_t local_time = mktime(&timeinfo); // Convert to local time
    this->epoch = local_time + timezone;
    this->timezone = timezone;
}
TimeInfo::TimeInfo(const int year,const int month,const int day,const int hour,const int min,const int sec,const int timezone) {
    struct tm timeinfo{};
    timeinfo.tm_year = year - 1900; // Years since 1900
    timeinfo.tm_mon = month;     // Month (0 - 11)
    timeinfo.tm_mday = day;          // Day of the month (1 - 31)
    timeinfo.tm_hour = hour;         // Hour (0 - 23)
    timeinfo.tm_min = min;        // Minute (0 - 59)
    timeinfo.tm_sec = sec;             // Second (0 - 59)
    timeinfo.tm_isdst = 0;           // Daylight Saving Time flag

    time_t local_time = mktime(&timeinfo); // Convert to local time
    this->epoch = local_time + timezone;
    this->timezone = timezone;
}


long TimeInfo::getEpoch() const {
    return epoch;
}

int TimeInfo::getTimeZone() const {
    return timezone;
}

void TimeInfo::modify(const long add) {
    epoch += add;
}


