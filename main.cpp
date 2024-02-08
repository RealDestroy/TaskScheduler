#include <vector>
#include "Tasks.h"



int main() {
    Command cmd1(Toggle,10);
    Command cmd2(Toggle,11);
    TimeInfo time1 = TimeInfo(2024,FEBRUARY,8,14,31,0,CST);
    TimeInfo time2 = TimeInfo(2024,FEBRUARY,8,14,31,0,CST);
    std::vector<ExecutableTask*> tasks;

    ExecutableTask t1(time1,10,cmd1);
    ExecutableTask t2(time2,cmd2);

    tasks.push_back(&t1);
    tasks.push_back(&t2);

    while(true) {
        for(auto task : tasks) {
            if(task->canExecute()){
                task->execute();
            }
        }
        sleep(1);
    }
    return 0;
}

