#include "SchedulerAPI/TaskScheduler.h"

int main() {
    TaskScheduler scheduler; //runs on its own thread
    scheduler.start();
    return 0;
}



