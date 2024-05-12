#include <string>

using namespace std;

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
};