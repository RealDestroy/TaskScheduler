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
void TaskScheduler::readInSchedules() {
    std::string filename =  TaskScheduler::cwd() + "\\SchedulerAPI\\schedules_test.json";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }

    // Read the entire file content into a string
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Parse the JSON string into a RapidJSON document
    rapidjson::Document document;
    if (document.Parse(jsonContent.c_str()).HasParseError()) {
        std::cerr << "Error: Failed to parse JSON" << std::endl;
    }

    std::vector<unsigned int> task_ids;
    if(document.HasMember(TASK_IDS) && document[TASK_IDS].IsArray()) {
        for(auto& id : document[TASK_IDS].GetArray()) {
            if(id.IsUint()) {
                task_ids.emplace_back(id.GetUint());
            }
        }
        for(auto id : task_ids) {
            std::string id_s = std::to_string(id);
            assert(id_s.size() == 1);
            const char id_c = id_s[0];
            if(document.HasMember(&id_c) && document[&id_c].IsString()) {
                std::string packet = document[id].GetString();
                //parse packet
                //"id:time_zone:start_time:interval:command_type:remote_device"
                ExecutableTask& task = TaskScheduler::parsePacket(packet);
                std::cout << task.string() << std::endl;
            }
        }
    }

}
void TaskScheduler::start() {
    MAIN_THREAD = std::thread(&TaskScheduler::loop,this);
    MAIN_THREAD.join(); //wait until thread ends (never unless stopped manually)
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

ExecutableTask &TaskScheduler::parsePacket(std::string& packet_s) {
    std::vector<std::string> packet = JSONUtility::split(packet_s,":");
    unsigned long task_id =          std::stoul(packet.at(0));
    int time_zone =                  std::stoi(packet.at(1));
    unsigned long long start_time =  std::stoll(packet.at(2));
    unsigned long interval =         std::stoul(packet.at(3));
    std::string command_type =       packet.at(4);
    int device_id =                  std::stoi(packet.at(5));
    Command command(Command::getType(command_type),device_id);
    TimeInfo time(start_time,timezone);
    ExecutableTask task(time,interval,command);
    return task;
    //"id:time_zone:start_time:interval:command_type:remote_device"
}





