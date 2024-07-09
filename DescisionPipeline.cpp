#include <iostream>
#include <set>
#include <limits>
#include <algorithm>
#include <iterator>
#include "DescisionPipeline.hpp"

/**
 * @brief Returns set of student-names from given map with closest amount of points <= <leqPoints>
 *
 * @param leqPoints point border to search for downwards
 * @param studs map for search
 * @return std::set<std::string>
 */
std::set<std::string> DescisionPipeline::closestLEQPointsStudents(uint8_t leqPoints, std::map<std::string, uint8_t> studs)
{
    // Find students with points equal leqPoints
    std::set<std::string> closestStuds;
    for (auto it = studs.begin(); it != studs.end(); ++it)
    {
        if (this->csvMan.getStudent(it->first)->getPoints() == leqPoints)
            closestStuds.insert(it->first);
    }
    // return when Students found, otherwise recur method with leqPoints -1 if possible
    if (closestStuds.empty() && leqPoints > 0)
    {
        return closestLEQPointsStudents(--leqPoints, studs);
    }
    else
    {
        return closestStuds;
    }
}
/**
 * @brief Returns set of student-names from given map with closest amount of points >= <geqPoints>
 *
 * @param geqPoints point border to search for upwards
 * @param studs map for search
 * @return std::set<std::string>
 */
std::set<std::string> DescisionPipeline::closestGEQPointsStudents(uint8_t geqPoints, std::map<std::string, uint8_t> studs)
{
    // Find students with points equal leqPoints
    std::set<std::string> closestStuds;
    for (auto it = studs.begin(); it != studs.end(); ++it)
    {
        if (this->csvMan.getStudent(it->first)->getPoints() == geqPoints)
            closestStuds.insert(it->first);
    }
    // return when Students found, otherwise recur method with greaterPoints + 1 (and geqPoints no overflow to terminate)
    if (closestStuds.empty() && (uint8_t)(geqPoints + 1) >= geqPoints)
    {
        return closestGEQPointsStudents(++geqPoints, studs);
    }
    else
    {
        return closestStuds;
    }
}

/**
 * @brief Eliminates all students from selection, whose difference to the preferred score is too high
 *
 * For example: With a preferred score of 1, all students with a score other than 1 are discarded.
 * If there is no such student, the next lowest result is used.
 * If there are no students with score below <preferredPoints> than the next highest score is
 * preferred until a result is found.
 *
 * @param preferredPoints preferred number of points
 */
void DescisionPipeline::rulePreferredPoints(uint8_t preferredPoints)
{
    // Find students to remain
    std::set<std::string> remainingStuds = closestLEQPointsStudents(preferredPoints, this->studPriorizing);
    // Check if students were found that have preferred points
    if (remainingStuds.empty())
    {
        remainingStuds = closestGEQPointsStudents(preferredPoints + 1, this->studPriorizing);
    }
    // Discard students from map that are not remaining students
    for (auto it = this->studPriorizing.begin(); it != this->studPriorizing.end();)
    {
        std::string studName = it->first;
        if (remainingStuds.find(studName) == remainingStuds.end()) // if studName not exists in remainStuds
            it = this->studPriorizing.erase(it);                   // iterator points to position after erased element
        else
            it++;
    }
}
/**
 * @brief Adds <priorityValue> to priority count of each student belonging in the seminar.
 *
 * @param semGroup current seminar group
 * @param priorityValue value added to priority count
 */
void DescisionPipeline::rulePriorizeCorrectSemGroup(std::string semGroup, uint8_t priorityValue)
{
    for (auto it = this->studPriorizing.begin(); it != this->studPriorizing.end(); ++it)
    {
        std::string studName = it->first;
        // students semGroup equals current semGroup?
        if (this->csvMan.getStudent(studName)->getSemGroup() == semGroup)
        {
            this->studPriorizing.at(studName) += priorityValue; // increase priority
        }
    }
}
/**
 * @brief Adds <priorityValue> to priority count of repeaters. Repeaters will be recognized by the
 * 2nd digit of their seminar group (2nd digit of the year), since it has to differ for being
 * repeater.
 *
 * @param semGroup current seminar group
 * @param priorityValue value added to priority count
 */
void DescisionPipeline::rulePriorizeRepeaters(std::string semGroup, uint8_t priorityValue)
{
    for (auto it = this->studPriorizing.begin(); it != this->studPriorizing.end(); ++it)
    {
        std::string studName = it->first;
        std::string studSemGroup = this->csvMan.getStudent(studName)->getSemGroup();
        // Repeaters seminar group differ guaranteed in second digit of the year (XYINB-Z)
        if (studSemGroup.at(1) != semGroup.at(1))
        {
            this->studPriorizing.at(studName) += priorityValue; // increase priority
        }
    }
}
/**
 * @brief Removes all students except those sitting furthest in front.
 *
 */
void DescisionPipeline::ruleFurthestInFront()
{
    // collect current selection of students as set
    std::set<std::string> studSelection;
    for (auto stud : studPriorizing)
    {
        studSelection.insert(stud.first);
    }

    // determine which students in row(ASC) are contained in selection
    std::set<std::string> studsInRow;
    std::set<std::string> priorizedStudsInRow;
    for (int i = 0; priorizedStudsInRow.empty(); i++)
    {
        auto row = input->studSelection.find(i);
        if (row != input->studSelection.end())
        { // check if row has value
            studsInRow = row->second;
            std::set_intersection(
                studSelection.begin(), studSelection.end(),
                studsInRow.begin(), studsInRow.end(),
                std::inserter(priorizedStudsInRow, priorizedStudsInRow.begin())); // Inserter for Not-Read-Only Iterator
        }
    }

    // discard all students not in priorizedStudsInRow (= studSelection - priorizedStudsInRow)
    std::set<std::string> discardStuds;
    std::set_difference(
        studSelection.begin(), studSelection.end(),
        priorizedStudsInRow.begin(), priorizedStudsInRow.end(),
        std::inserter(discardStuds, discardStuds.begin())); // Inserter for Not-Read-Only Iterator
    for (std::string studName : discardStuds)
    {
        studPriorizing.erase(studName);
    }
}

DescisionPipeline::DescisionPipeline(InputStruct const *input) : csvMan(CSVManager(input->csvFile)), input(input)
{
    // create <map> from students
    this->studPriorizing = {};
    for (auto studRow : input->studSelection)
    {
        for (std::string studName : studRow.second)
        {
            // does given student exist?
            if (csvMan.getStudent(studName) != nullptr)
                this->studPriorizing.insert({studName, 0});
            else if (input->verbose)
                std::cout << "Student \"" << studName << "\" existiert nicht." << std::endl;
        }
    }
}