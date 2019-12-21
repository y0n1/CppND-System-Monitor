#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

#include "linux_parser.h"

using std::string;
using std::vector;

const string kTotalProcesses = "processes";
const string kRunningProcesses = "procs_running";

int getProcessesStatistics(const string processKind) {
  int stats {};
  string label, value, line;
  std::ifstream fileStream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);
  if (fileStream.is_open()) {
    do {
      std::getline(fileStream, line);
    } while (line.rfind(processKind, 0) != 0);

    std::istringstream linestream(line);
    linestream >> label >> value;
    stats = std::stoi(value);
  }

  return stats;
}

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
  string os, kernel, version;
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
        int pid = std::stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float memInUse {};
  string line, label, memTotal, memFree;
  std::ifstream fileStream {kProcDirectory + kMeminfoFilename};

  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream1 {line};
    lineStream1 >> label >> memTotal;
    
    std::getline(fileStream, line);
    std::istringstream lineStream2 {line};
    lineStream2 >> label >> memFree;

    memInUse = 1.0f - std::stof(memFree) / std::stof(memTotal);
  }

  return memInUse;
}

long LinuxParser::UpTime() {
  long upTime {};
  string line, upTimeSeconds;
  std::ifstream fileStream {kProcDirectory + kUptimeFilename};
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream (line);
    lineStream >> upTimeSeconds;
    upTime = (long)floor(std::stof(upTimeSeconds));
  }

  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() {
  return getProcessesStatistics(kTotalProcesses);
}

int LinuxParser::RunningProcesses() {
  return getProcessesStatistics(kRunningProcesses);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }