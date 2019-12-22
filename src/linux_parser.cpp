#include <dirent.h>
#include <unistd.h>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

const string kTotalProcesses = "processes";
const string kRunningProcesses = "procs_running";

int getSystemWideProcessesStatistics(const string processKind) {
  int stats{};
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
  float memInUse{};
  string line, label, memTotal, memFree;
  std::ifstream fileStream{kProcDirectory + kMeminfoFilename};

  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream1{line};
    lineStream1 >> label >> memTotal;

    std::getline(fileStream, line);
    std::istringstream lineStream2{line};
    lineStream2 >> label >> memFree;

    memInUse = 1.0f - std::stof(memFree) / std::stof(memTotal);
  }

  return memInUse;
}

long LinuxParser::UpTime() {
  long upTime{};
  string line, upTimeSeconds;
  std::ifstream fileStream{kProcDirectory + kUptimeFilename};
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    lineStream >> upTimeSeconds;
    upTime = (long)floor(std::stof(upTimeSeconds));
  }

  return upTime;
}

long LinuxParser::Jiffies() {
  vector<string> cpuData = LinuxParser::CpuUtilization();
  long totalJiffies{};
  for (size_t i = 0; i < cpuData.size(); i++) {
    totalJiffies += std::stol(cpuData[i]);
  }

  return totalJiffies;
}

long LinuxParser::ActiveJiffies() {
  vector<string> cpuData = LinuxParser::CpuUtilization();

  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

long LinuxParser::IdleJiffies() {
  vector<string> cpuData = LinuxParser::CpuUtilization();

  return std::stol(cpuData[kIdle_]) + std::stol(cpuData[kIOwait_]);
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuData{};
  std::ifstream fileStream{kProcDirectory + kStatFilename};
  if (fileStream.is_open()) {
    string line, label;
    string user, nice, system, idle, iowait, irq, softirq, steal, guest,
        guest_nice;
    std::getline(fileStream, line);
    std::istringstream lineStream{line};
    lineStream >> label >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
    string values[]{user, nice,    system, idle,  iowait,
                    irq,  softirq, steal,  guest, guest_nice};
    for (auto& value : values) {
      cpuData.push_back(value);
    }
  }

  return cpuData;
}

int LinuxParser::TotalProcesses() {
  return getSystemWideProcessesStatistics(kTotalProcesses);
}

int LinuxParser::RunningProcesses() {
  return getSystemWideProcessesStatistics(kRunningProcesses);
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  long utime, stime, cutime, cstime;
  std::ifstream fileStream{LinuxParser::kProcDirectory + "/" +
                           std::to_string(pid) + "/" +
                           LinuxParser::kStatFilename};
  std::getline(fileStream, line);
  std::istringstream lineStream{line};
  for (int i = 1; i <= 17; i++) {
    lineStream >> value;
    if (i == 14) {
      utime = std::stol(value);
    } else if (i == 15) {
      stime = std::stol(value);
    } else if (i == 16) {
      cutime = std::stol(value);
    } else if (i == 17) {
      cstime = std::stol(value);
    }
  }

  return utime + stime + cutime + cstime;
}

string LinuxParser::Command(int pid) {
  string command;
  std::ifstream fileStream{kProcDirectory + "/" + std::to_string(pid) + "/" +
                           kCmdlineFilename};
  if (fileStream.is_open()) {
    std::getline(fileStream, command);
  }

  return command;
}

string LinuxParser::Ram(int pid) {
  string label, ram, line;
  std::ifstream fileStream{kProcDirectory + "/" + std::to_string(pid) + "/" +
                           kStatusFilename};
  if (fileStream.is_open()) {
    do {
      std::getline(fileStream, line);
    } while (line.rfind("VmSize", 0) != 0);
    std::istringstream lineStream{line};
    lineStream >> label >> ram;
  }

  return std::to_string((stoi(ram) / 1024));
}

string LinuxParser::Uid(int pid) {
  string label, realUID, line;
  std::ifstream fileStream{LinuxParser::kProcDirectory + "/" +
                           std::to_string(pid) + "/" +
                           LinuxParser::kStatusFilename};
  if (fileStream.is_open()) {
    do {
      std::getline(fileStream, line);
    } while (line.rfind("Uid", 0) != 0);
    std::istringstream lineStream{line};
    lineStream >> label >> realUID;
  }

  return realUID;
};

string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string userName, password, userID, line;
  std::ifstream fileStream{LinuxParser::kPasswordPath};
  if (fileStream.is_open()) {
    do {
      std::getline(fileStream, line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream{line};
      lineStream >> userName >> password >> userID;
    } while (userID.rfind(uid, 0) != 0);
  }

  return userName;
}

long LinuxParser::UpTime(int pid) {
  string line, value;
  long starttime;
  std::ifstream fileStream{LinuxParser::kProcDirectory + "/" +
                           std::to_string(pid) + "/" +
                           LinuxParser::kStatFilename};
  std::getline(fileStream, line);
  std::istringstream lineStream{line};
  for (int i = 1; i <= 22; i++) {
    lineStream >> value;
    if (i == 22) {
      starttime = std::stol(value);
    }
  }

  return starttime;
}