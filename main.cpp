#include "SchedulerAPI/TaskScheduler.h"

int main() {
    TaskScheduler scheduler; //runs on its own thread
    //scheduler.readInSchedules();
    scheduler.start();
    //shouldn't get past here
    return 0;
}



