//
// Created by destroy on 2/10/24.
//


#include "TaskScheduler.h"


void TaskScheduler::loop() {
    while(!HALT_SCHEDULER) {
        for(auto& task :  TaskScheduler::tasks) {
            if(task->canExecute()) {
                task->execute();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); //sleep loop for 1 second
    }
}
void TaskScheduler::task_handler() const {
    Command cmd1(Toggle,10);
    Command cmd2(Toggle,11);
    TimeInfo time1 = TimeInfo(2024,FEBRUARY,12,1,0,0,CST);
    TimeInfo time2 = TimeInfo(2024,FEBRUARY,12,1,0,0,CST);

    ExecutableTask t1(time1,10,cmd1);
    ExecutableTask t2(time2,cmd2);

    //add(t1);
    //add(t2);
    //save();
    while(!HALT_SCHEDULER) {
        //save();
        std::this_thread::sleep_for(std::chrono::seconds(10)); //sleep loop for 1 second
    }
}


void TaskScheduler::start() {
    UTILITY_THREAD = std::thread(&TaskScheduler::task_handler,this);
    MAIN_THREAD = std::thread(&TaskScheduler::loop,this);
    MAIN_THREAD.join(); //wait until thread ends (never unless stopped manually)
    UTILITY_THREAD.join(); //ends when MAIN_THREAD ends
}
void TaskScheduler::stop() {
    HALT_SCHEDULER = true;
}

void TaskScheduler::add(ExecutableTask& task) {
    std::lock_guard<std::mutex> lock(taskLock);
    tasks.push_back(&task);
}

std::string TaskScheduler::cwd() {
    std::string substring = PROJECT_NAME;
    std::string dir = std::filesystem::current_path().string();
    size_t found = dir.find(substring);
    if (found != std::string::npos) {
        return dir.substr(0, found + substring.length());
    }
    return dir;
}

ExecutableTask TaskScheduler::parsePacket(std::string& packet_s) {
    std::vector<std::string> packet = JSONUtility::split(packet_s,":");
    unsigned long task_id =          std::stoul(packet.at(0));
    int time_zone =                  std::stoi(packet.at(1));
    unsigned long long start_time =  std::stoll(packet.at(2));
    unsigned long interval =         std::stoul(packet.at(3));
    std::string command_type =       packet.at(4);
    int device_id =                  std::stoi(packet.at(5));
    Command command(Command::getType(command_type),device_id);
    TimeInfo time(start_time,time_zone);
    return ExecutableTask(task_id,time,interval,command);
    //"id:time_zone:start_time:interval:command_type:remote_device"
}

void TaskScheduler::save() {
    rapidjson::GenericDocument<rapidjson::UTF8<>> doc;
    doc.SetObject();
    doc.AddMember(TASK_IDS, task_ids.size(), doc.GetAllocator());
    for(ExecutableTask* task : tasks) {
        rapidjson::Value key;
        std::string id_s = std::to_string(task->getId());
        key.SetString(id_s.c_str(),doc.GetAllocator());
        rapidjson::Value value;
        std::string s = task->string();
        value.SetString(s.c_str(),s.length());
        doc.AddMember(key,value, doc.GetAllocator());
    }

    // Convert JSON document to string
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* jsonStr = buffer.GetString();

    // Write JSON string to file
    std::ofstream outFile(file());
    if (outFile.is_open()) {
        outFile.flush();
        outFile << jsonStr;
        outFile.close();
        std::cout << "JSON data has been written to schedules.json" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
        return;
    }
}
void TaskScheduler::readInSchedules() {
    std::ifstream file(TaskScheduler::file());

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << TaskScheduler::file() << std::endl;
        return;
    }

    // Read the entire file content into a string
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Parse the JSON string into a RapidJSON document
    rapidjson::Document document;
    if (document.Parse(jsonContent.c_str()).HasParseError()) {
        std::cerr << "Error: Failed to parse JSON" << std::endl;
        return;
    }

    std::vector<unsigned int> task_ids_new;
    if(document.HasMember(TASK_IDS) && document[TASK_IDS].IsUint()) {
        unsigned int length = document[TASK_IDS].GetUint();
        for(auto i = 0; i < length; i++) {
            if(document.HasMember(std::to_string(i).c_str()) && document[i].IsString()) {
                std::string packet = document[i].GetString();
                //parse packet
                //"id:time_zone:start_time:interval:command_type:remote_device"
                ExecutableTask task = TaskScheduler::parsePacket(packet);
                std::cout << task.string() << std::endl;
                add(task);
            }
        }
    }
}

std::string TaskScheduler::file() {
    return TaskScheduler::cwd() + "\\SchedulerAPI\\schedules.json";
}
