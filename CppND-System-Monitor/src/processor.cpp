#include "processor.h"
#include "linux_parser.h"
using std::vector;
using std::string;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    return LinuxParser::CpuUtilization(); }