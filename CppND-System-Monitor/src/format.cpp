#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hr = (int)(seconds/3600);
    int min = ((int)(seconds/60))%60;
    int sec = (int)(seconds%60);
    string time = std::to_string(hr) + ':' + std::to_string(min) + ':' + std::to_string(sec);
    return time; }