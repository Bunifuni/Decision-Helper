#pragma once
#include <string>
#include <map>
#include "InputStruct.hpp"
#include "CSVManager.hpp"

class DescisionPipeline
{
    friend class DescisionPipelineTest;

private:
    CSVManager csvMan;
    InputStruct const *input;
    std::map<std::string, uint8_t> studPriorizing; // Map students on points

    std::set<std::string> closestLEQPointsStudents(uint8_t leqPoints, std::map<std::string, uint8_t> studs);
    std::set<std::string> closestGEQPointsStudents(uint8_t geqPoints, std::map<std::string, uint8_t> studs);

    void rulePreferredPoints(uint8_t preferredPoints);
    void rulePriorizeRightSemGroup(std::string semGroup, uint8_t priorityValue);

public:
    DescisionPipeline(InputStruct const *input);
};