#pragma once
#include <vector>
#include <map>
#include <string>

#define CSVFILE "students.csv"

/**
 * @brief Enumeration to distinguish the command for the program.
 */
enum ProgramCommand
{
    unhandled,
    decision,
    increment,
    decrement
};

/**
 * @brief Struct which holds the necessary information from preprocessing of thee input.
 * 
 */
struct InputStruct
{
    std::string csvFile = CSVFILE;
    ProgramCommand state = unhandled;

    bool verbose = false;
    bool allowRepeater = true;
    uint8_t preferredPoints = 0;
    uint8_t priorityCorrectSemGroup = 2;
    uint8_t priorityRepeater = 1;
    std::string semGroup = "";

    std::map<int, std::set<std::string>> studSelection;
};
