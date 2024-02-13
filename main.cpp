#include "SchedulerAPI/TaskScheduler.h"

int main() {
    Command cmd1(Toggle,10);
    Command cmd2(Toggle,11);
    TimeInfo time1 = TimeInfo(2024,FEBRUARY,12,1,0,0,CST);
    TimeInfo time2 = TimeInfo(2024,FEBRUARY,12,1,0,0,CST);

    ExecutableTask t1(time1,10,cmd1);
    ExecutableTask t2(time2,cmd2);

    TaskScheduler scheduler; //runs on its own thread
    TaskScheduler::readInSchedules();
    //scheduler.add(t1);
    //scheduler.add(t2);

    //scheduler.start();
    //shouldn't get past here
    return 0;
}



