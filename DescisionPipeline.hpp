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
    std::map<std::string, uint8_t> studPriorizing; // Map students on a 'priorize value'

    std::set<std::string> closestLEQPointsStudents(uint8_t leqPoints, std::map<std::string, uint8_t> studs);
    std::set<std::string> closestGEQPointsStudents(uint8_t geqPoints, std::map<std::string, uint8_t> studs);
    uint8_t getMaxPriorizing();
    void removeLessPriorizedThen(uint8_t priorizeValue);
    void removeLeastPriorized();
    void removeRepeaters(std::string semGroup);
    std::string getRandomStudent();
    void listStrings(std::set<std::string> const &listingSet);
    void listStrings(std::map<std::string, uint8_t> const &listingMap);

    void rulePreferredPoints(uint8_t preferredPoints);
    void rulePriorizeCorrectSemGroup(std::string semGroup, uint8_t priorityValue);
    void rulePriorizeRepeaters(std::string semGroup, uint8_t priorityValue);
    void ruleFurthestInFront();

public:
    DescisionPipeline(InputStruct const *input);
    Student *decideForStudent();
    void incrementPointsOfSelection();
    void decrementPointsOfSelection();
};

std::string padTo(std::string const &str, const size_t num, const char paddingChar = ' ');
