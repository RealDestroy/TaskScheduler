//
// Created by destroy on 2/10/24.
//
#ifdef _WIN32
#define LINUX false
#endif

#ifdef __unix__
#define LINUX true
#endif

#include "TaskScheduler.h"


void TaskScheduler::loop() {
    while(!HALT_SCHEDULER) {
        for(auto& task :  TaskScheduler::tasks) {
            std::cout << task->string() << std::endl;
            if(task->canExecute()) {
                task->execute();
            }
            if(task->isComplete() || task->isCancelled()) {
                std::destroy_at(task);
                //remove any null tasks
                tasks.erase( std::remove( std::begin(tasks), std::end(tasks), nullptr ), std::end(tasks));
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(5)); //sleep loop for 1 second
    }
}

void TaskScheduler::task_handler() {
    using namespace std;
    //retrieveLocalSchedules();
    //retrieveCloudSchedules();
    Command cmd1(Toggle,10);
    Command cmd2(Toggle,11);
    TimeInfo time1 = TimeInfo(2024,FEBRUARY,12,1,0,0,CST);
    TimeInfo time2 = TimeInfo(2024,FEBRUARY,12,1,0,0,CST);

    ExecutableTask t1(0,time1,10,cmd1);
    ExecutableTask t2(1,time2,0,cmd2);

    add(t1);
    add(t2);
    save();

    while(!HALT_SCHEDULER) {
        std::this_thread::sleep_for(std::chrono::seconds(10)); //sleep loop for 10 second
        //bool new_data = retrieveCloudSchedules(); //get all cloud schedules and return how many were new
        //if(new_data) {
            //save();
            //cout << "Saved new data" << endl;
        //}
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
bool TaskScheduler::add(ExecutableTask& task ) {
    unsigned int id = task.getId();
    if(SchedulerUtil::contains(task_ids,id)) {
        //std::cout << "already added " << id << std::endl;
        return false;
    }

    taskLock.lock();
    tasks.push_back(&task);
    task_ids.push_back(task.getId());
    std::cout << "added " << task.getId() << std::endl;
    taskLock.unlock();
    return true;
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
    using namespace std;
    std::vector<std::string> packet = SchedulerUtil::split(packet_s,":");
    unsigned long long task_id =          std::stoull(packet.at(0));
    int time_zone =                  std::stoi(packet.at(1));
    unsigned long long start_time =  std::stoull(packet.at(2));
    unsigned long long interval =         std::stoull(packet.at(3));
    int command_type =               std::stoi(packet.at(4));
    int device_id =                  std::stoi(packet.at(5));

    Command_Type cmd_type = Command::getCommandTypeOf(command_type);
    Command command(cmd_type,device_id);
    TimeInfo time_info(start_time,time_zone);

    ExecutableTask t(task_id,time_info,interval,command);
    return t;
    //"id:time_zone:start_time:interval:command_type:remote_device"
}

void TaskScheduler::save() {
    rapidjson::GenericDocument<rapidjson::UTF8<>> doc;
    doc.SetObject();
    doc.AddMember(TASK_IDS, task_ids.size(), doc.GetAllocator());
    for(ExecutableTask* task : tasks) {
        rapidjson::Value key(std::to_string(task->getId()).c_str(),doc.GetAllocator());
        rapidjson::Value value(task->string().c_str(),doc.GetAllocator());
        doc.AddMember(key,value, doc.GetAllocator());
    }

    // Convert JSON document to string
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* jsonStr = buffer.GetString();

    // Write JSON string to file
    std::ofstream outFile(local_file());
    if (outFile.is_open()) {
        outFile.flush();
        outFile << jsonStr;
        outFile.close();
        std::cout << "JSON data has been written to local_schedules.json" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
        return;
    }
}
void TaskScheduler::retrieveLocalSchedules() {
    using namespace std;
    using namespace rapidjson;

    std::ifstream file(TaskScheduler::local_file());

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

    if(document.HasMember(TASK_IDS) && document[TASK_IDS].IsUint()) {
        unsigned int length = document[TASK_IDS].GetUint();
        for(auto i = 0; i < length; i++) {
            const char* i_p = SchedulerUtil::toChar(i);
            if(document.HasMember(i_p) && document[i_p].IsString()) {
                std::string packet = document[i_p].GetString();
                //parse packet
                //"id:time_zone:start_time:interval:command_type:remote_device"
                ExecutableTask task = TaskScheduler::parsePacket(packet);
                std::cout << task.string() << std::endl;
                add(task);
            }
        }
    }
}
bool TaskScheduler::retrieveCloudSchedules() {
    using namespace std;
    using namespace rapidjson;
    std::ifstream file(TaskScheduler::file());

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << TaskScheduler::file() << std::endl;
        return false;
    }

    // Read the entire file content into a string
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Parse the JSON string into a RapidJSON document
    rapidjson::Document doc;
    if (doc.Parse(jsonContent.c_str()).HasParseError()) {
        std::cerr << "Error: Failed to parse JSON" << std::endl;
        return false;
    }

    //The system has a static ID which is created at first installation,
    // this will be used to parse the json
    const char* ID = "1";
    const char* DEVICE_ID = "deviceID";
    const char* SCHEDULES = "schedules";
    int scheduleID, deviceID, day, action = 0;
    int new_schedules = 0;
    if(doc.HasMember(ID) && doc[ID].IsArray()) {
        //cout << ID << " is an array" << endl;
        for(const auto& v : doc[ID].GetArray()) {
            if(v.HasMember(DEVICE_ID) && v[DEVICE_ID].IsNumber()) {
                //cout << "Has device id: " << v[DEVICE_ID].GetInt() << endl;
            }
            if(v.HasMember(SCHEDULES) && v[SCHEDULES].IsArray()) {
                for(const auto& sc : v[SCHEDULES].GetArray()) {
                    if(sc.IsObject()) {
                        const auto& schedule = sc.GetObject();

                        if(schedule.HasMember("scheduleID") && schedule["scheduleID"].IsInt()) {
                            scheduleID = schedule["scheduleID"].GetInt();
                            //cout << "Has schedule id: " << schedule["scheduleID"].GetInt() << endl;
                        }
                        if(schedule.HasMember("deviceID") && schedule["deviceID"].IsInt()) {
                            deviceID = schedule["deviceID"].GetInt();
                            //cout << "Has device id: " << schedule["deviceID"].GetInt() << endl;
                        }
                        if(schedule.HasMember("dayOfWeek") && schedule["dayOfWeek"].IsInt()) {
                            day = schedule["dayOfWeek"].GetInt();
                            //cout << "Has day: " << schedule["dayOfWeek"].GetInt() << endl;
                        }
                        if(schedule.HasMember("action") && schedule["action"].IsInt()) {
                            action = schedule["action"].GetInt();
                            //cout << "Has action: " << schedule["action"].GetInt() << endl;
                        }
                        if(schedule.HasMember("time") && schedule["time"].IsString()) {
                            std::string time = schedule["time"].GetString();

                            TimeInfo t_info(time,day);
                            Command command(Command::getCommandTypeOf(action),deviceID);

                            ExecutableTask task(scheduleID,t_info,command);
                            //
                            //add the task to the loop
                            //cout << "Has time: " << schedule["time"].GetString() << endl;
                            bool n = add(task);
                            new_schedules += add(task);
                            if(n) {
                                cout << task.string() << endl;
                            }
                        }
                    }
                }
            }
        }
    }
    return new_schedules > 0;
}

std::string TaskScheduler::file() {
    if(LINUX) {
        return TaskScheduler::cwd() + R"(\SchedulerAPI\schedules\schedules.json)";
    } else {
        return TaskScheduler::cwd() + R"(/SchedulerAPI/schedules/schedules.json)";
    }

}
std::string TaskScheduler::local_file() {
    if(LINUX) { //check if system is linux or windows (slash compatibility for files)
        return TaskScheduler::cwd() + R"(\SchedulerAPI\schedules\local_schedules.json)";
    } else {
        return TaskScheduler::cwd() + R"(/SchedulerAPI/schedules/local_schedules.json)";
    }
}

