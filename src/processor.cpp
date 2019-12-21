#include <cmath>

#include "linux_parser.h"
#include "processor.h"

float Processor::Utilization() {
  long activeJiffies = LinuxParser::ActiveJiffies();
  float deltaActiveJiffies =
      (float)std::abs(this->lastActiveJiffies_ - activeJiffies);
  this->lastActiveJiffies_ = activeJiffies;

  long jiffies = LinuxParser::Jiffies();
  float deltaJiffies = (float)std::abs(this->lastJiffies_ - jiffies);
  this->lastJiffies_ = jiffies;

  return deltaActiveJiffies / deltaJiffies;
}