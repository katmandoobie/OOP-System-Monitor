#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string Mem, key, kb, line;
  float MemTotal,MemFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(getline(stream,line)){
      std::istringstream linestream(line);
      linestream >> key >> Mem >> kb;
      if(key == "MemTotal:") MemTotal = stof(Mem);
      if(key == "MemFree:") {
        MemFree = stof(Mem);
        break;
      }
    }
    return (MemTotal - MemFree)/MemTotal;
  }
  return 0.0;
}

//Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string uptime,idle;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if(stream.is_open())
  {
    getline(stream,line);
    std::istringstream linestream(line);
    linestream >> uptime;
    
  }
  return stol(uptime); 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long jiffies;
  vector<string> CpuUtil = LinuxParser::CpuUtilization();
  for(auto i: CpuUtil) jiffies += stol(i);
  return jiffies; }

//Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string values;
  string line;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open())
  {
    getline(stream,line);
    std::istringstream linestream(line);
    for(int i = 0; i < 17; i++) {
      linestream >> values;
      jiffies.push_back(values);
    }
    long total = stol(jiffies[16]) + stol(jiffies[15]) + stol(jiffies[14]) + stol(jiffies[13]);
    return total;
  }
  
  return 0; }

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> jiffies = CpuUtilization();
  return stol(jiffies[kUser_]) + stol(jiffies[kNice_]) + stol(jiffies[kSystem_]); 
}

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  return stol(jiffies[kIdle_]) + stol(jiffies[kIOwait_]); 
}

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, val, key;
  vector<string> values;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> val){
      values.emplace_back(val);
    }
    return values;
  }
  
  return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while(getline(stream, line))
  {
    std::istringstream linestream(line);
    linestream >> key >> val;
    if(key == "processes") return stoi(val);
  }
  return 0; }

//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while(getline(stream, line))
  {
    std::istringstream linestream(line);
    linestream >> key >> val;
    if(key == "procs_running") return stoi(val);
  }
  return 0; }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  getline(stream,cmd);
  return cmd; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line,key,val;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while(getline(stream, line))
  {
    std::istringstream linestream(line);
    linestream >> key >> val;
    if(key == "VmSize:"){
      if(std::all_of(val.begin(),val.end(), isdigit))
        return val;
      else
        return to_string(0);
    }
  }
  
  return to_string(0); 
}

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, val;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while(getline(stream,line))
  {
    std::istringstream linestream(line);
    linestream >> key >> val;
    if(key == "Uid:") return val;
  }
  return string(); 
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, user, junk;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    for(int i = 1; i <= 9; i++)
    {
        getline(stream,line);
        
    }
    std::istringstream linestream(line);
    linestream >> junk >> user;
    return user;
  }
  
  return string(); }

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string uptime, line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open())
  {
    getline(stream,line);
    std::istringstream linestream(line);
    for(int i = 0; i < 22; i++){
      linestream >> uptime;
    }
    
    return stol(uptime);

  }
  return 0; }

  
