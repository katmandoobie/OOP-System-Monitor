#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// constructor
Process::Process(int pid)
{
    Pid_ = pid;
    cmd_ = LinuxParser::Command(pid);
    user_ = LinuxParser::User(pid);
}


int Process::Pid() { return Pid_; }
float Process::CpuUtilization() {
    //calc previous values of cpu jiffies
    prevIdle_ = prevIdle_ + prevIOWait_;
    prevNonIdle_ = prevUser_ + prevNice_ + prevSystem_ + prevIRQ_ + prevSoftIRQ_ + prevSteal_;
    prevTotal_ = prevIdle_ + prevNonIdle_;

    //get the current values of cpu jiffies
    vector<string> jiffs = LinuxParser::CpuUtilization();
    float idle = stof(jiffs[LinuxParser::kIdle_]) + stof(jiffs[LinuxParser::kIOwait_]);
    float nonidle = stof(jiffs[LinuxParser::kUser_]) + stof(jiffs[LinuxParser::kNice_]) + stof(jiffs[LinuxParser::kSystem_]) + stof(jiffs[LinuxParser::kIRQ_]) + stof(jiffs[LinuxParser::kSoftIRQ_]) + stof(jiffs[LinuxParser::kSteal_]);
    float total = idle + nonidle;

    //calc delta of current and previous values
    float totalDelta = total - prevTotal_;
    float idleDelta = idle - prevIdle_;

    //set CpuUtilization_ as percentage
    CpuUtilization_ = ((totalDelta - idleDelta)*100)/totalDelta;

    //update previous values with new current values and return CpuUtilization_
    prevIOWait_ = stof(jiffs[LinuxParser::kIOwait_]);
    prevUser_ = stof(jiffs[LinuxParser::kUser_]);
    prevNice_ = stof(jiffs[LinuxParser::kNice_]);
    prevSystem_ = stof(jiffs[LinuxParser::kSystem_]);
    prevIRQ_ = stof(jiffs[LinuxParser::kIRQ_]);
    prevSoftIRQ_ = stof(jiffs[LinuxParser::kSoftIRQ_]);
    prevSteal_ = stof(jiffs[LinuxParser::kSteal_]);
    return  CpuUtilization_;
}
string Process::Command() { 
    return cmd_; 
}
string Process::Ram() {
    ram_ = stoi(LinuxParser::Ram(Pid_))/1024;
    return to_string(ram_); 
}
string Process::User() { 
    return user_;
}
long int Process::UpTime() {     
    UpTime_ =  LinuxParser::UpTime(Pid_);
    return UpTime_;
}
bool Process::operator<(Process const& a) const {
    return this->ram_ < a.ram_ ? true : false;}