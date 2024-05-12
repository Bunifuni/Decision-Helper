#include <string>
#include <Student.hpp>

using namespace std;

Student::Student(string name, string semGroup, uint8_t points)
{
    this->name = name;
    this->semGroup = semGroup;
    this->points = points;
}

string Student::getName()
{
    return this->name;
}

string Student::getSemGroup()
{
    return this->semGroup;
}

uint8_t Student::getPoints()
{
    return this->points;
}
