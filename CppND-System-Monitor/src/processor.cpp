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
  int count = 0 ;
  long value ;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> name){
      if (name == "cpu"){
        while(count < 10){
          linestream >> val;
          std::stringstream string1(val);
          string1 >> value;
          vals.push_back(value);
          count++ ;
        }
      }
    }
  }
  for(int i=0 ; i< 10 ; i ++){
    util_times[i] = vals[i];
  }
  
  long total_jiffies = LinuxParser::Jiffies();
  long idle_jiffies = LinuxParser::IdleJiffies();
  // sleep(5);
  // long idle_jiffies_p = kIdle_p + kIOwait_p ;
  // long total_jiffies_p = kUser_p + kNice_p + kSystem_p + kIRQ_p + kSoftIRQ_p + kSteal_p + idle_jiffies_p;
   long idle_jiffies_p = util_times[3] + util_times[4];
   long total_jiffies_p = util_times[0] + util_times[1] + util_times[2] + util_times[5] + util_times[6] + util_times[7];
  // long idle_jiffies_p = LinuxParser::IdleJiffies();;
  // long total_jiffies_p = LinuxParser::Jiffies();
  long totald = total_jiffies - total_jiffies_p;
  long idled = idle_jiffies - idle_jiffies_p;

  float cpu_u = ((float)(totald - idled)/(float)totald);
  return cpu_u; }