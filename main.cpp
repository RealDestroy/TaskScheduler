

#include <iostream>
#include <vector>
#include "Tasks.h"

int main() {
    std::vector<ExecutableTask> tasks;
    ExecutableTask t1(getTime(2024,2,7,15,26,0));
    //std::cout << "Current Time: " << readableTime(getCurrentTime()) << std::endl;
    //std::cout << "Time: " << readableTime(t1.getStartTime()) << std::endl;
    //std::cout << "diff: " << t1.getStartTime() - getCurrentTime() << std::endl;
    tasks.push_back(t1);


    while (true) {
        for(ExecutableTask task : tasks) {
            long time = getCurrentTime();
            std::cout << readableTime(time) << std::endl;
            if(task.getStartTime() <= time) {
                task.execute();
            }
        }
        sleep(1);
    }

    return 0;
}

