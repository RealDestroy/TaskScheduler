#include <iostream>
#include <vector>
#include "Tasks.h"

int main() {
    std::vector<ExecutableTask*> tasks;
    ExecutableTask t1(getTime(2024,2,7,20,14,0),5);
    ExecutableTask t2(getTime(2024,2,7,22,30,0));

    tasks.push_back(&t1);
    tasks.push_back(&t2);


    while(true) {
        for(auto task : tasks) {
            long time = getCurrentTime();
            if (task->getStartTime() <= time) {
                task->execute();
            }
        }
        sleep(1);
    }


    return 0;
}

