#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return this->cpu_; }

// get PIDs of each process and store in vector<Process> Processes_
// sort each process by Ram Utilization and return
vector<Process>& System::Processes() {
  processes_.clear();
  vector<int> pids = LinuxParser::Pids();
  for (auto i : pids) {
    Process process(i);
    process.CpuUtilization();
    process.UpTime();
    processes_.emplace_back(process);
  }
  std::sort(processes_.begin(), processes_.end(), std::less<Process>());
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
