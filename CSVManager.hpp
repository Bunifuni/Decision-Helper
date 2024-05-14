#pragma once
#include <vector>
#include "Student.hpp"

class CSVManager
{
private:
    std::string filename;
    std::vector<Student> students;
    Student createStudentFromCSV(char *csvLine, size_t size);
    vector<Student> readCSV(string filename);
    void writeCSV(string filename);
    void changePoints(string name, bool incr);

public:
    CSVManager(string filename);
    Student *getStudent(string name);
    void incrementPoints(string name);
    void decrementPoints(string name);
};