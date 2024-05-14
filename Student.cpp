#include <iostream>
#include <cstring>
#include <stdexcept>
#include "Student.hpp"

#define DELIMITER ",\n"
#define COLUMN_NAME 0
#define COLUMN_SEMGROUP 1
#define COLUMN_POINTS 2

using namespace std;

/**
 * @brief Construct a new Student object from concrete values
 *
 * @param name name of student
 * @param semGroup seminar group of student
 * @param points points of student
 */
Student::Student(string name, string semGroup, uint8_t points)
{
    this->name = name;
    this->semGroup = semGroup;
    this->points = points;
}

/**
 * @brief Construct a new Student object from csvLine
 *
 * @param csvLine string in csv-format with student information
 */
Student::Student(char *csvLine, size_t size)
{
    char *funcArg = new char[size];             // stores argument for exception-handling because argument will be altered
    copy(csvLine, csvLine + size + 1, funcArg); // deep copy of function argument (+1 for copying string-end aka \0)

    const char *delimiter = DELIMITER;
    vector<string> tokens = separateLine(csvLine, delimiter);
    try
    {
        this->name = tokens.at(COLUMN_NAME);
        this->semGroup = tokens.at(COLUMN_SEMGROUP);
        this->points = (uint8_t)stoi(tokens.at(COLUMN_POINTS));
    }
    catch (std::out_of_range &exc)
    {
        cerr << "Error:\t" << exc.what() << "\tat creating Student-Obj with:\n\t\""
             << funcArg << "\"" << endl;
        throw;
    }
    catch (std::invalid_argument &excia)
    {
        cerr << "Error:\t" << excia.what() << "\tat creating Student-Obj with invalid arg for points:\n\t\""
             << funcArg << "\"" << endl;
        throw;
    }
}

/**
 * @brief returns name of student
 *
 * @return string
 */
string Student::getName()
{
    return this->name;
}

/**
 * @brief returns seminar group of student
 *
 * @return string
 */
string Student::getSemGroup()
{
    return this->semGroup;
}

/**
 * @brief returns points of student
 *
 * @return uint8_t
 */
uint8_t Student::getPoints()
{
    return this->points;
}

/**
 * @brief Returns points as string
 * 
 * @return string 
 */
string Student::getPointsAsStr()
{
    return std::to_string(this->points);
}

/**
 * @brief Increments student's points by 1
 */
void Student::incrementPoints()
{
    this->points++;
}

/**
 * @brief Decrements student's points by 1
 */
void Student::decrementPoints()
{
    if (this->points > 0)
    {
        this->points--;
    }
    else
    {
        std::cout << "Warning: student " << this->name << " has no points to lose (already 0 points)" << std::endl;
    }
}

/**
 * @brief Separates given line by given delimiter and returns list of separated strings
 *
 * @param line string to separate
 * @param delimiter delimiter to separate by
 * @return vector<string>
 */
vector<string> separateLine(char *line, const char *delimiter)
{
    vector<string> words;                  // vector to collect strings
    char *token = strtok(line, delimiter); // char* until delimiter
    while (token != nullptr)
    {
        string str = token;     // string instead of char* for string methods (string cannot be nullptr)
        if (str.back() == '\n') // does str end with newline(LF)?
        {
            str.pop_back(); // remove newline
        }
        words.push_back(str);               // add word to list
        token = strtok(nullptr, delimiter); // next word
    }
    return words;
}