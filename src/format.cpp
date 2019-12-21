#include <cstdio>
#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long totalSeconds) {
  int seconds = totalSeconds % 60;
  int minutes = (totalSeconds / 60) % 60;
  int hours = (totalSeconds / 3600) % 24;

  const string OUTPUT_FORMAT{"HH:MM:SS"};
  char buffer[OUTPUT_FORMAT.length() + 1];
  sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
  string uptime{buffer};

  return uptime;
}