#include "processor.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
using std::stof;
using std::stol;

#include "linux_parser.h"
#include "process.h"

/*
 Made reference from following link..
 https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
prevsteal NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald

 */

float Processor::Utilization() {
  current_jdata = LinuxParser::Jiffies();
  current_idata = LinuxParser::IdleJiffies();

  final_jdata = current_jdata - previous_jdata;
  final_idata = current_idata - previous_idata;

  previous_jdata = final_jdata;
  previous_idata = final_idata;

  float CpuPercentage = (float)(final_jdata - final_idata) / final_idata;

  return CpuPercentage;
}
