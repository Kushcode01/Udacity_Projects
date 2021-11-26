#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long util_times[10];

};

#endif