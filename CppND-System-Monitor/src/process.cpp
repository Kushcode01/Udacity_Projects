#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"


using std::string;
using std::to_string;
using std::vector;
Process::Process(int pid){
      pid_ = pid;
      cpuUtilization_ = CpuUtilization();
}
// TODO: Return this process's ID
int Process::Pid() {

    return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    
    //float cpuUtilization_ = 0.0;
    float syc_clk_tck = sysconf(_SC_CLK_TCK);
    long upTime = LinuxParser::UpTime();  // test
    long upTimeProcess = LinuxParser::UpTime(pid_);
    long totalJiffies = LinuxParser::ActiveJiffies(pid_);
    float duration_sec = upTime - (upTimeProcess/syc_clk_tck);
    float cpuUtilization_ = ((totalJiffies/ syc_clk_tck) /(duration_sec));
    return (cpuUtilization_); }

// TODO: Return the command that generated this process
string Process::Command() { 
    string command_p = LinuxParser::Command(pid_);
    if(command_p.length()>20){
        command_p.resize(20);
        command_p.append("....");
    }
    return command_p;  }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    string user_p = LinuxParser::User(pid_);
    if(user_p.length()>6){
        user_p.resize(6);
        user_p.append("+");
    }
    return user_p;
     }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {         
    return (cpuUtilization_ < a.cpuUtilization_); }