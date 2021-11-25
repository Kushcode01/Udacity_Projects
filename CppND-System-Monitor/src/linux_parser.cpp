#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
std::vector<long> util_times(10, 0.0);

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
  long mem_tot, mem_avl, mem_free, buffer;
  string Mem_tot, Mem_avl, Mem_free, Buffer, type, val;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while (linestream >> type >> val){
      if (type == "MemTotal"){
        std::stringstream string1(val);
        string1 >> mem_tot ; 
      }
      if (type == "MemFree"){
        std::stringstream string1(val);
        string1 >> mem_free ; 
      }
      if (type == "MemAvailable"){
        std::stringstream string1(val);
        string1 >> mem_avl ; 
      }
      if (type == "Buffers"){
        std::stringstream string1(val);
        string1 >> buffer ; 
      }
    }
    

  }
  return (((mem_tot - mem_free)/mem_tot)*100);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::stringstream linestream(line);
    linestream >> uptime;
    return uptime;

  }

  return 0.0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line, name, val;
  long jiffies = 0.0;
  vector<long> vals;
  int count = 0 ;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> name){
      if (name == "cpu"){
        while (linestream >> val){
          std::stringstream string1(val);
          string1 >> vals[count];
          count ++ ;
        }
      }
      else{
        continue;
      }

    }
  }
  for (int i:vals){
    jiffies+=vals[i];
  } 
  return jiffies; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 

  string line, name, val;
  long utime, stime, cutime, cstime ;
  long p_jiffies = 0.0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    int count = 0 ;
    while (linestream >> val){
      if(count == 13){
        std::stringstream string1(val);
        string1 >> utime; 
      }
      if(count == 14){
        std::stringstream string1(val);
        string1 >> stime; 
      }
      if(count == 15){
        std::stringstream string1(val);
        string1 >> cutime; 
      }
      if(count == 16){
        std::stringstream string1(val);
        string1 >> cstime; 
      }
      
      count++;
    }
  }
  p_jiffies = utime + stime +cutime + cstime;
  
  return p_jiffies; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line, name, val;
  long active_jiffies = 0.0;
  vector<long> vals;
  int count = 0 ;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> name){
      if (name == "cpu"){
        while (linestream >> val){
          std::stringstream string1(val);
          string1 >> vals[count];
          count ++ ;
        }
      }
      else{
        continue;
      }

    }
  }
  for (int i=0; i < 10; i++){
    if((i == 3) || (i==4)){
      continue;
    }
    active_jiffies += vals[i];
  } 
  return active_jiffies; 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line, name, val;
  long idle_jiffies = 0.0;
  vector<long> vals;
  int count = 0 ;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> name){
      if (name == "cpu"){
        while (linestream >> val){
          std::stringstream string1(val);
          string1 >> vals[count];
          count ++ ;
        }
      }
      else{
        continue;
      }
    }
  }
  for (int i=0; i<10; i++){
    if((i == 3) || (i==4)){
      idle_jiffies += vals[i];
    }
  } 
  return idle_jiffies; 
  }

// TODO: Read and return CPU utilization --------------------------------------------------------------
float LinuxParser::CpuUtilization() { 
  
  long total_jiffies = Jiffies();
  long idle_jiffies = IdleJiffies();
  // long idle_jiffies_p = kIdle_p + kIOwait_p ;
  // long total_jiffies_p = kUser_p + kNice_p + kSystem_p + kIRQ_p + kSoftIRQ_p + kSteal_p + idle_jiffies_p;
  long idle_jiffies_p = util_times[3] + util_times[4];
  long total_jiffies_p = util_times[0] + util_times[1] + util_times[2] + util_times[5] + util_times[6] + util_times[7];
  long totald = total_jiffies - total_jiffies_p;
  long idled = idle_jiffies - idle_jiffies_p;

  float cpu_u = ((totald - idled)/totald);
  string line, name, val;
  vector<long> vals;
  int count = 0 ;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> name){
      if (name == "cpu"){
        while(count < 10){
          linestream >> val;
          std::stringstream string1(val);
          string1 >> vals[count];
          count++ ;
        }
      }
    }
  }
  for(int i=0 ; i< 10 ; i ++){
    util_times.push_back(vals[i]);
  }

  return cpu_u; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, name, val; 
  int processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> name){
      if (name == "processes"){
        while (linestream >> val){
          std::stringstream string1(val);
          string1 >> processes;
          return processes;
        }
      }
      else{
        continue;
      }

    }
  }
  return 0;}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { string line, name, val; 
  int processes_running;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> name){
      if (name == "processes"){
        while (linestream >> val){
          std::stringstream string1(val);
          string1 >> processes_running;
          return processes_running;
        }
      }
      else{
        continue;
      }

    }
  }
  return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream (kProcDirectory + to_string(pid) + kCmdlineFilename);
  std::getline(stream,line);
  return line; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, type, mem_u;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while(stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while (linestream >> type){
      if (type == "VmSize"){
        long mem_mb, mem_kb;
        linestream >> mem_u;
        std::stringstream string1(mem_u);
        string1 >> mem_kb;
        mem_mb = mem_kb/1024;
        return to_string(mem_mb);
        
      }
      else{
        continue;
      }
    }
  }
  return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, name, uid;
  std::ifstream stream (kProcDirectory + to_string(pid) + kStatusFilename);
  while (stream.is_open()){
    std::getline(stream , line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while(linestream >> name){
      if (name == "Uid"){
        linestream >> uid;
        return uid;
      }
      else {
        continue;
      }
    }

  }
  return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, user, uid_read, x;
  string uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  while(stream.is_open()){
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    // int count = 0;
    // for (int i = 0 ; i < line.length() ; i++){
    //   if (line[i] == ":"){
    //     count++;
    //     if (count == 2){
    //       while(line[i] != ":"){
    //         uid_read.append(line[i]);
    //       }
    //     }
    //   }
    // }
    std::istringstream linestream(line);
    while (linestream >> user >> x >> uid_read){
      if (uid_read == uid){
        return user;
      }
      else{
        continue;
      }
    }

  }

  return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, val;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  while(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    int count = 0;
    while( linestream >> val){
      if(count == 21){
        std::stringstream stream1(val);
        long up_time;
        stream1 >> up_time;
        return up_time/sysconf(_SC_CLK_TCK);
      }
      count++;
    }

  }
  return 0; }

  float LinuxParser::CpuUtilizationP(int pid){
    string line, val;
    long utime, stime, cutime, cstime, starttime, total_time, seconds;
    float cpu;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    while(stream.is_open()){
      std::getline(stream, line);
      std::istringstream linestream(line);
      int count = 0;
      while( linestream >> val){
        if(count == 13){
          std::stringstream stream1(val);
          stream1 >> utime;
        }
        if(count == 14){
          std::stringstream stream1(val);
          stream1 >> stime;
        }
        if(count == 15){
          std::stringstream stream1(val);
          stream1 >> cutime;
        }
        if(count == 16){
          std::stringstream stream1(val);
          stream1 >> cstime;
        }
        if(count == 21){
          std::stringstream stream1(val);
          stream1 >> starttime;
          starttime = starttime/sysconf(_SC_CLK_TCK);
        }
        count++;
      }
      
  }

  total_time = utime + stime + cutime + cstime;
  seconds = UpTime() - starttime;
  cpu = (((total_time/sysconf(_SC_CLK_TCK))/seconds)*100);
  return cpu;

  }