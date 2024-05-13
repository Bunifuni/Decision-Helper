#include <iostream>
#include <vector>
#include "Student.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // cout << "Hello World" << endl;
    char csvLine[] = "MMuster,21INB-1,2\n";
    Student subject1 = Student(csvLine, sizeof(csvLine));
    cout << subject1.getName() << "\n"
         << subject1.getSemGroup() << "\n"
         << to_string(subject1.getPoints()) << endl;
    return 0;
}