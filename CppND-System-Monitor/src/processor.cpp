#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
using std::vector;
using std::string;
// TODO: Return the aggregate CPU utilization
// float Processor::Utilization() { 
//     return LinuxParser::CpuUtilization(); }

float Processor::Utilization() { 

  string line, name, val;
  vector<long> vals;
  float cpu_u = 0.0;
  int count = 0 ;
  long value ;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()){
    while ( std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "cpu"){
        while(linestream >> val){
          value = std::stoi(val);
          vals.push_back(value);
        }
      }
    }
  }
  
  long active_jiffies_p = LinuxParser::ActiveJiffies();
  long idle_jiffies_p = LinuxParser::IdleJiffies();
  sleep(1);
  // long idle_jiffies_p = kIdle_p + kIOwait_p ;
  // long total_jiffies_p = kUser_p + kNice_p + kSystem_p + kIRQ_p + kSoftIRQ_p + kSteal_p + idle_jiffies_p;
  // long idle_jiffies = vals[3] + vals[4];
  // long total_jiffies = vals[0] + vals[1] + vals[2] + vals[5] + vals[6] + vals[7];
  long idle_jiffies = LinuxParser::IdleJiffies();;
  long active_jiffies = LinuxParser::ActiveJiffies();
  long actived = (active_jiffies - active_jiffies_p);
  long idled = (idle_jiffies - idle_jiffies_p);

  cpu_u = ((float)(actived)/(float)(actived + idled));
  return cpu_u; }