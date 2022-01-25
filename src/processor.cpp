#include "processor.h"
#include <vector>

#include "linux_parser.h"

using namespace std;
// Return the aggregate CPU utilization
// add number of jiffies system is actually working
// and divide by total jiffies system has been running
float Processor::Utilization() {
  vector<string> util = LinuxParser::CpuUtilization();
  float total = 0;
  float active = std::stof(util[0]) + std::stof(util[1]) + std::stof(util[2]);
  for (auto i : util) total = total + std::stof(i);

  return (active / total);
}
