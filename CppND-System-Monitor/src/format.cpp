#include <string>

#include "format.h"
#include <vector>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hr = (seconds/3600);
    long min = ((seconds/60))%60;
    long sec = (seconds%60);
    string HH = std::to_string(hr);
    string MM = std::to_string(min);
    string SS = std::to_string(sec);
    
    // std::vector<string> time_vec = {HH, MM, SS};
    // for (string &i : time_vec){
    //     if (i.size() == 1){
    //         char c = i[0];
    //         string s = "0";
    //         s.append(std::to_string(c));
    //         i = s;
    //     }
    // }
    if( HH.size() == 1){
        string HH_mod;
        HH_mod.append("0");
        HH_mod.push_back(HH.back());
        HH = HH_mod;
    }
    if( MM.size() == 1){
    string MM_mod;
    MM_mod.append("0");
    MM_mod.push_back(MM.back());
    MM = MM_mod;
    }
    if( SS.size() == 1){
    string SS_mod;
    SS_mod.append("0");
    SS_mod.push_back(SS.back());
    SS = SS_mod;
    }

    string time = HH + ':' + MM + ':' + SS;
    return time; }