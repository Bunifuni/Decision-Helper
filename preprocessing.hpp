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
const std::set<std::string> increaseArgAliases = {"add", "++"};

/**
 * @brief Aliases for decreasing argument
 */
const std::set<std::string> decreaseArgAliases = {"sub", "--"};

int preprocessing(int argc, char *argv[], InputStruct *input);
int processOpts(int argc, char *argv[], InputStruct *input);
std::map<int, std::set<std::string>> processSelectionStr(char *selectionStr);
std::vector<std::string> separateLine(char *line, const char *delimiter);
std::vector<std::pair<std::string, int>> splitElements(std::vector<std::string> unsplitVector, const char *delimiter);
