#pragma once
#include <set>
#include <string>
#include <vector>
#include "InputStruct.hpp"

/**
 * @brief Enumeration to distinguish the result of the preprocessing.
 */
enum processingResult
{
    error,
    unhandled,
    decision,
    increment,
    decrement
};
/**
 * @brief Aliases for increasing argument
 */
const std::set<std::string> increaseArgs = {"add", "++"};

/**
 * @brief Aliases for decreasing argument
 */
const std::set<std::string> decreaseArgs = {"sub", "--"};

int preprocesssing(int argc, char *argv[]);
InputStruct *processOpts(int argc, char *argv[]);
std::map<int, std::set<std::string>> processSelectionStr(std::string selectionStr);
std::vector<std::string> separateLine(char *line, const char *delimiter);