#include <vector>
#include <string>

#define CSVFILE "students.csv"

struct InputStruct {
    bool verbose = false;
    uint8_t preferredPoints = 0;
    std::string csvFile = CSVFILE;
    std::vector<std::string> studSelection;
};