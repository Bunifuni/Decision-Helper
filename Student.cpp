#include <iostream>
#include "Student.hpp"
#include "preprocessing.hpp"

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
