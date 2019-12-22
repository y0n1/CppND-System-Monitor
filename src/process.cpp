#include <unistd.h>
#include <cctype>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  user_ = "";
  command_ = "";
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  float totalTime = LinuxParser::ActiveJiffies(pid_);
  float hertz = sysconf(_SC_CLK_TCK);
  float seconds =
      (float)LinuxParser::UpTime() - (float)LinuxParser::UpTime(pid_) / hertz;
  float cpuUsage = totalTime / hertz / seconds;

  return cpuUsage;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process& that) {
  return this->CpuUtilization() < that.CpuUtilization();
}