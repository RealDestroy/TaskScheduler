
#include "SchedulerAPI/Tasks.h"


int main() {
    std::vector<ExecutableTask*> tasks;
    Command cmd1(Toggle,10);
    Command cmd2(Toggle,11);
    TimeInfo time1 = TimeInfo(2024,FEBRUARY,9,14,24,0,CST);
    TimeInfo time2 = TimeInfo(2024,FEBRUARY,9,14,24,0,CST);

    ExecutableTask t1(time1,10,cmd1);
    ExecutableTask t2(time2,cmd2);

    tasks.push_back(&t1);
    tasks.push_back(&t2);



    while(true) {
        for(auto task : tasks) {
            if(task->isCancelled() || task->isComplete() || task->isPastExecution()) {
                //std::cout << task->getId() << " deleted." << std::endl;
                delete task;
                continue;
            }
            if(task->canExecute()){
                task->execute();
            }
        }
        sleep(1);
    }

    //shouldn't get past here


    return 0;
}


