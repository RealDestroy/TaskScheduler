//
// Created by Destroy on 2/7/2024.
//

#ifndef TASKSCHEDULER_DATEUTILITY_H
#define TASKSCHEDULER_DATEUTILITY_H

#endif //TASKSCHEDULER_DATEUTILITY_H

#define HST  (-36000)
#define AKST (-32400)
#define PST  (-28800)
#define MST  (-25200)
#define CST  (-21600)
#define EST  (-18000)

#define SUNDAY 0
#define MONDAY 1
#define TUESDAY 2
#define WEDNESDAY 3
#define THURSDAY 4
#define FRIDAY 5
#define SATURDAY 6

#define JANUARY 0
#define FEBRUARY 1
#define MARCH 2
#define APRIL 3
#define MAY 4
#define JUNE 5
#define JULY 6
#define AUGUST 7
#define SEPTEMBER 8
#define OCTOBER 9
#define NOVEMBER 10
#define DECEMBER 11

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
long getTime(int year, int month, int day, int hour, int minute, int second,int timezone);
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
long getTime(int day, int hour, int minute, int second,int timezone);
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
long getCurrentTime(int timezone);
class TimeInfo {
public:
    TimeInfo();
    explicit TimeInfo(int timezone);
    TimeInfo(int day,int hour,int min,int sec,int timezone);
    TimeInfo(int year,int month,int day,int hour,int min,int sec,int timezone);
    int getTimeZone() const;
    long getEpoch() const;
    void modify(long add);
private:
    long epoch;
    int timezone;
};

