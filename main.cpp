#include <vector>
#include "Tasks.h"


int main() {
    Command cmd1(Toggle,10);
    Command cmd2(Toggle,11);
    std::vector<ExecutableTask*> tasks;

    ExecutableTask t1(TimeInfo(2024,FEBRUARY,8,23,53,30,CST),10,cmd1);
    ExecutableTask t2(TimeInfo(2024,FEBRUARY,8,23,53,30,CST),cmd2);

    tasks.push_back(&t1);
    tasks.push_back(&t2);

    while(true) {
        for(auto task : tasks) {
            long time = getCurrentTime(CST);
            if (task->getStartTime() == time) {
                task->execute();
            }
        }
        sleep(1);
    }
    return 0;
}

