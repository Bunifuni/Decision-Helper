#pragma once
#include <vector>
#include <map>
#include <string>

#define CSVFILE "students.csv"

/**
 * @brief Enumeration to distinguish the result of the preprocessing.
 */
enum ProcessingResult
{
    unhandled,
    decision,
    increment,
    decrement
};

struct InputStruct
{
    std::string csvFile = CSVFILE;
    ProcessingResult state = unhandled;

    bool verbose = false;
    uint8_t preferredPoints = 0;
    uint8_t priorityCorrectSemGroup = 2;
    uint8_t priorityRepeater = 1;
    std::string semGroup = "";

    std::map<int, std::set<std::string>> studSelection;
};
