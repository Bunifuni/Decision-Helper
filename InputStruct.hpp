#pragma once
#include <vector>
#include <string>

#define CSVFILE "students.csv"

struct InputStruct {
    std::string csvFile = CSVFILE;
    
    bool verbose = false;
    uint8_t preferredPoints = 0;
    std::string semGroup = "";

    std::map<int, std::set<std::string>> studSelection;
};