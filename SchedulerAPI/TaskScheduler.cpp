//
// Created by destroy on 2/10/24.
//


#include "TaskScheduler.h"


void TaskScheduler::loop() {
    while(!HALT_SCHEDULER) {
        for(auto& task :  TaskScheduler::tasks) {
            std::cout << task->string() << std::endl;
            if(task->canExecute()) {
                task->execute();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(5)); //sleep loop for 1 second
    }
}

void TaskScheduler::task_handler() {
    retrieveLocalSchedules();
    //retrieveCloudSchedules();

    Command cmd1(Toggle,10);
    Command cmd2(Toggle,11);
    TimeInfo time1 = TimeInfo(2024,FEBRUARY,12,1,0,0,CST);
    TimeInfo time2 = TimeInfo(2024,FEBRUARY,12,1,0,0,CST);

    ExecutableTask t1(0,time1,10,cmd1);
    ExecutableTask t2(1,time2,0,cmd2);

    add(t1);
    add(t2);
    //save();

    while(!HALT_SCHEDULER) {
        std::this_thread::sleep_for(std::chrono::seconds(10)); //sleep loop for 10 second
        //save();
        //retrieveCloudSchedules();
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
void TaskScheduler::add(ExecutableTask& task ) {
    unsigned int id = task.getId();
    if(SchedulerUtil::contains(task_ids,id)) {
        std::cout << "already added " << id << std::endl;
        return;
    }

    taskLock.lock();
    tasks.push_back(&task);
    task_ids.push_back(task.getId());
    std::cout << "added " << task.getId() << std::endl;
    taskLock.unlock();
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

            if(document.HasMember(std::to_string(i).c_str()) && document[std::to_string(i).c_str()].IsString()) {
                std::string packet = document[std::to_string(i).c_str()].GetString();
                //parse packet
                //"id:time_zone:start_time:interval:command_type:remote_device"
                ExecutableTask task = TaskScheduler::parsePacket(packet);
                std::cout << task.string() << std::endl;
                add(task);
            }
        }
    }
}
void TaskScheduler::retrieveCloudSchedules() {
    using namespace std;
    using namespace rapidjson;
    std::ifstream file(TaskScheduler::file());

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << TaskScheduler::file() << std::endl;
        return;
    }

    // Read the entire file content into a string
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Parse the JSON string into a RapidJSON document
    rapidjson::Document doc;
    if (doc.Parse(jsonContent.c_str()).HasParseError()) {
        std::cerr << "Error: Failed to parse JSON" << std::endl;
        return;
    }

    for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr){
        //cout << itr->name.GetString() << endl;
    }

    /*
    for (auto it : doc.FindMember("schedules")) {
        cout << it->name.GetString() << endl;

        if (it->name.IsNumber()) {
            cout << "Key: " << it->name.GetString() << endl;

            const Value& arr = it->value;

            for (SizeType i = 0; i < arr.Size(); i++) {
                const Value& obj = arr[i];

                int deviceID = obj["deviceID"].GetInt();
                cout << "  Device ID: " << deviceID << endl;

                const Value& schedules = obj["schedules"];

                for (SizeType j = 0; j < schedules.Size(); j++) {
                    const Value &schedule = schedules[j];

                    int scheduleID = schedule["scheduleID"].GetInt();
                    int dayOfWeek = schedule["dayOfWeek"].GetInt();
                    const string &time = schedule["time"].GetString();
                    //convert from "08:00:00", to epoch time
                    std::vector<std::string> packet = SchedulerUtil::split(time,":");
                    int hours = std::stoi(packet[0]);
                    int minutes = std::stoi(packet[1]);
                    int seconds = std::stoi(packet[2]);
                    int month = getCurrentMonth();
                    int year = getCurrentYear();
                    int dayOfMonth = getNextMonthDay(dayOfWeek);
                    int action = schedule["action"].GetInt();

                    TimeInfo t_info(year,month,dayOfMonth,hours,minutes,seconds,CST);
                    Command command(Command::getCommandTypeOf(action),scheduleID);


                    ExecutableTask task(t_info,command);

                    cout << task.string() << endl;

                    add(task);

                }
            }
        }


    }
     */
}

std::string TaskScheduler::file() {
    return TaskScheduler::cwd() + R"(\SchedulerAPI\schedules\schedules.json)";
}
std::string TaskScheduler::local_file() {
    return TaskScheduler::cwd() + R"(\SchedulerAPI\schedules\local_schedules.json)";
}


