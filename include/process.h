#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int Pid_;
  std::string cmd_;
  std::string user_;
  float CpuUtilization_;
  long int UpTime_;
  int ram_;

  // previous values of cpu jiffies
  float prevIdle_ = 0;
  float prevTotal_ = 0;
  float prevNonIdle_ = 0;
  float prevIOWait_ = 0;
  float prevUser_ = 0;
  float prevNice_ = 0;
  float prevSystem_ = 0;
  float prevIRQ_ = 0;
  float prevSoftIRQ_ = 0;
  float prevSteal_ = 0;
};

#endif