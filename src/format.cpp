#include <string>

#include "format.h"

using std::string;


string Format::ElapsedTime(long seconds) { 
    long min = seconds/60;
    long hour = seconds/3600;
    string HH = std::to_string(hour);
    string MM = std::to_string(min%60);
    string SS = std::to_string(seconds%60);
    SS.insert(0,2-SS.length(), '0');
    return HH + ":" + MM + ":" + SS; }