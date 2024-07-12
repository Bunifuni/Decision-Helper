#pragma once
#include <set>

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
const std::set<const char *> increaseArgs = {"add", "++"};

/**
 * @brief Aliases for decreasing argument
 */
const std::set<const char *> decreaseArgs = {"sub", "--"};

int preprocesssing(int argc, char *argv[]);