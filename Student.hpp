#pragma once
#include <string>
#include <vector>

using namespace std;

class Student
{
private:
    string name;
    string semGroup;
    uint8_t points;

public:
    Student(string name, string semGroup, uint8_t points);
    Student(char *csvLine, size_t size);
    string getName();
    string getSemGroup();
    uint8_t getPoints();
};
vector<string> separateLine(char *line, const char *delimiter);