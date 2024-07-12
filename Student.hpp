#pragma once
#include <string>

using namespace std;

/**
 * @brief Object that holds the necessary information for a student
 */
class Student
{
private:
    string name;
    string semGroup;
    uint8_t points;

public:
    Student(string name, string semGroup, uint8_t points);
    string getName();
    string getSemGroup();
    uint8_t getPoints();
    string getPointsAsStr();
    void incrementPoints();
    void decrementPoints();
};