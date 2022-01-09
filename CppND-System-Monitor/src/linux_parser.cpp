#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "format.h"

using std::stof;
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
  return string();
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string type, val;
  string line;
  vector<long> mem_list;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {

    for (int i =0; i <2 ;i++) {
    std::getline(stream,line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> type >> val;
    mem_list.push_back(std::stoi(val));
    }

    float memPercentage;
    memPercentage = 
            (float)(mem_list[0] - mem_list[1]) /
        (float)mem_list[0]; 
    return memPercentage;

  }
  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string up_time;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;

  }
  uptime = std::stoi(up_time);

  return uptime; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long jiffies_sum;
  vector<string> jiffies = LinuxParser::CpuUtilization(); 
  for (auto i :
       jiffies)  
  {
    jiffies_sum += std::stoi(i); 
  }

  return jiffies_sum; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 

  string line, name, val;
  vector<string> pid_stat = {};
  // long utime, stime, cutime, cstime ;
  long p_jiffies = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i =0 ; i < 17 ; i++){
      linestream >> val ;
      pid_stat.push_back(val);

    }
  }
  p_jiffies = std::stoi(pid_stat[13]) + std::stoi(pid_stat[14]) + std::stoi(pid_stat[15]) + std::stoi(pid_stat[16]);
  
  return p_jiffies; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active_jiffies = 0;
  vector<string> jiffies = CpuUtilization();

  long user = std::stoi(jiffies[CPUStates::kUser_]);
  long nice = std::stoi(jiffies[CPUStates::kNice_]);
  long irq = std::stoi(jiffies[CPUStates::kIRQ_]);
  long sirq = std::stoi(jiffies[CPUStates::kSoftIRQ_]);
  long system = std::stoi(jiffies[CPUStates::kSystem_]);
  long steal = std::stoi(jiffies[CPUStates::kSteal_]);
  active_jiffies = user + nice + irq + sirq + system + steal ;
    
  return active_jiffies; 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 

  long idle_jiffies = 0;
  vector<string> jiffies = LinuxParser::CpuUtilization();

  long idle = std::stoi(jiffies[CPUStates::kIdle_]);
  long iowait = std::stoi(jiffies[CPUStates::kIOwait_]);
  idle_jiffies = idle + iowait;

  return idle_jiffies; 
  }

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string values;
  vector<string> cpuN;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line))  
    {
      std::istringstream linestream(line);
      linestream >> key;  
      if (key == "cpu") {
        while (linestream >> values)  
        {                            
          cpuN.push_back(values);     
        }
      }
    }
  }
  return cpuN;
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, name, val; 
  int processes = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> name >> val;
      if (name == "processes"){
        processes = std::stoi(val);
        return processes;   
      }

    }
  }
  return processes;}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { string line, name, val; 
  int processes_running = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){;
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> name >> val;
      if (name == "procs_running"){
        processes_running = std::stoi(val);
        return processes_running;
      }

    }
  }
  return processes_running; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream (kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream,line);
    return line;
  }
  
  return line; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, type, mem_u;
  long mem_mb, mem_kb;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> type;
      if (type == "VmRSS"){
        linestream >> mem_u;
        mem_kb = std::stoi(mem_u);
        mem_mb = float(mem_kb)/float(1024);
        mem_u = to_string(mem_mb);
        return mem_u;
        break;
        
      }
    }
  }
  return mem_u;
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, name, uid;
  std::ifstream stream (kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){

    while(std::getline(stream , line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name;
      if (name == "Uid"){
        linestream >> uid;
        return uid;
        break;
      }
    }

  }
  return uid;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, user, uid_read, x;
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid_read;
      if (uid_read == uid){
        return user;
        break;
      }
    }

  }

  return user; }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, val;
  long up_time, uptime_process;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()){
    while( std::getline(stream, line)){
      std::istringstream linestream(line);
      for (int i = 0; i < 22 ; i++){
        linestream >> val;
      }
      up_time = std::stoi(val);
      long sys_uptime = LinuxParser::UpTime();
      uptime_process = (sys_uptime - ((up_time)/sysconf(_SC_CLK_TCK)));
      // string u_time = Format::ElapsedTime(uptime_process);
      return (uptime_process);
    }

  }
  return 0; }