#pragma once
#include <vector>
#include <string>

#define CSVFILE "students.csv"

struct InputStruct {
    std::string csvFile = CSVFILE;
    
    bool verbose = false;
    uint8_t preferredPoints = 0;
    uint8_t priorityCorrectSemGroup = 2;
    uint8_t priorityRepeater = 1;
    std::string semGroup = "";

    std::map<int, std::set<std::string>> studSelection;
};