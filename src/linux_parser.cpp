
#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::stol;

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
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization

float LinuxParser::MemoryUtilization() {
  float memoryTotal, memoryFree;
  string line, name, num;
  std::ifstream path(kProcDirectory + kMeminfoFilename);
  if (path.is_open()) {
    while (std::getline(path, line)) {
      std::istringstream strline(line);
      strline >> name >> num;
      if (name == "MemTotal:") {
        memoryTotal = stof(num);
      } else if (name == "MemFree:") {
        memoryFree = stof(num);
      }
    }
  }
  return ((memoryTotal - memoryFree) / memoryTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string data;
  long Time_str;

  std::ifstream path(kProcDirectory + kUptimeFilename);
  if (path.is_open()) {
    std::getline(path, data);
    std::istringstream strline(data);
    strline >> Time_str;
  }

  return Time_str;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string j_data;
  string cpu, user, nice, system, idle, iowait, irq, soft_irq, steal, guest,
      guest_nice;
  long total;
  std::ifstream path(kProcDirectory + kStatFilename);
  if (path.is_open()) {
    std::getline(path, j_data);
    std::istringstream data(j_data);
    data >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        soft_irq >> steal >> guest >> guest_nice;
    total = (stol(user) + stol(nice) + stol(system) + stol(idle) +
             stol(iowait) + stol(irq) + stol(soft_irq) + stol(steal) +
             stol(guest) + stol(guest_nice));

    return total;
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function

long LinuxParser::ActiveJiffies(int pid) {
  long total;
  string dataA, dataB;
  int x = 0;
  std::ifstream path(kProcDirectory + to_string(pid) + kStatFilename);
  if (path.is_open()) {
    std::getline(path, dataB);
    std::istringstream line_str(dataB);
    while (line_str >> dataA) {
      if (x >= 13 && x <= 16) {
        total += stol(dataA);
      }
      x++;
    }
  }
  return total;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string idle_data;
  string cpu, user, nice, system, idle, iowait, irq, soft_irq, steal, guest,
      guest_nice;
  float total;
  std::ifstream path(kProcDirectory + kStatFilename);
  if (path.is_open()) {
    std::getline(path, idle_data);
    std::istringstream data(idle_data);
    data >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        soft_irq >> steal >> guest >> guest_nice;
    total = stof(idle) + stof(iowait);
  }
  return total;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> List;
  string dataA, dataB;
  string cpu, user, nice, system, idle, iowait, irq, soft_irq, steal, guest,
      guest_nice;
  std::ifstream path(kProcDirectory + kStatFilename);

  if (path.is_open()) {
    std::getline(path, dataA);
    std::istringstream data(dataA);

    data >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        soft_irq >> steal >> guest >> guest_nice;

    List.push_back(user);
    List.push_back(nice);
    List.push_back(system);
    List.push_back(idle);
    List.push_back(iowait);
    List.push_back(irq);
    List.push_back(soft_irq);
    List.push_back(steal);
    List.push_back(guest);
    List.push_back(guest_nice);
  }
  return List;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key_data, value, line_data;
  std::ifstream path(kProcDirectory + kStatFilename);
  if (path.is_open()) {
    while (std::getline(path, line_data)) {
      std::istringstream line_str(line_data);
      line_str >> key_data >> value;
      if (key_data == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int RunningProcess = 0;
  string line, key_RunningProcess;
  std::ifstream path(kProcDirectory + kStatFilename);
  if (path.is_open()) {
    while (std::getline(path, line)) {
      std::istringstream line_str(line);
      line_str >> key_RunningProcess;
      if (key_RunningProcess == "procs_running") {
        line_str >> RunningProcess;
        return RunningProcess;
      }
    }
  }
  return 0;
}
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string data;
  std::ifstream path(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (path.is_open()) {
    std::getline(path, data);
  }
  return data;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string RAM, KEY, VALUE, LINE;
  std::ifstream path(kProcDirectory + to_string(pid) + kStatusFilename);
  if (path.is_open()) {
    while (std::getline(path, LINE)) {
      std::istringstream line_str(LINE);
      if (line_str >> KEY >> VALUE) {
        if (KEY == "VmSize:") {
          RAM = to_string(stol(VALUE) / 1024);
          return RAM;
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string KEY, VALUE, LINE;
  std::ifstream path(kProcDirectory + to_string(pid) + kStatusFilename);

  if (path.is_open()) {
    while (std::getline(path, LINE)) {
      std::istringstream line_stream(LINE);
      while (line_stream >> KEY >> VALUE) {
        if (KEY == "Uid:") {
          return to_string(stoi(VALUE) / 1024);
        }
      }
    }
  }

  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string strID, Data, USER, ID, lINE;
  strID = Uid(pid);
  std::ifstream path(kPasswordPath);
  if (path.is_open()) {
    while (std::getline(path, lINE)) {
      std::replace(lINE.begin(), lINE.end(), ':', ' ');
      std::istringstream line_str(lINE);
      line_str >> USER >> Data >> ID;
      if (ID == strID) {
        return USER;
      }
    }
  }

  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function

long int LinuxParser::UpTime(int pid) {
  string LINE, TIME;

  std::ifstream path(kProcDirectory + to_string(pid) + kStatFilename);
  if (path.is_open()) {
    while (std::getline(path, LINE)) {
      std::istringstream line_str(LINE);
      for (int x = 0; x <= 13; x++) {
        line_str >> TIME;
      }
    }

    return stol(TIME);
  }

  return 0;
}
