#include <string>
#include <map>
#include "InputStruct.hpp"
#include "CSVManager.hpp"

class DescisionPipeline
{
private:
    CSVManager csvMan;
    InputStruct const *input;
    std::map<std::string, uint8_t> studPriorizing;

public:
    DescisionPipeline(InputStruct const *input);
};