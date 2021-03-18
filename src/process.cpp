#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;
using namespace std;

Process::Process(int pid) { pid_ = pid; }

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization

float Process::CpuUtilization() const {
  long TotalTime, StartTime, UpTime, result;
  float sec, div;

  TotalTime = LinuxParser::ActiveJiffies(pid_);
  StartTime = LinuxParser::UpTime(pid_);
  UpTime = LinuxParser::UpTime();
  div = StartTime / sysconf(_SC_CLK_TCK);
  sec = UpTime - div;

  return (TotalTime / sysconf(_SC_CLK_TCK)) / sec;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  if (a.CpuUtilization() < CpuUtilization()) {
    return true;
  }
  return false;
}
