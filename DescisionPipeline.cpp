#include <iostream>
#include <set>
#include <limits>
#include <algorithm>
#include <iterator>
#include <random>
#include <time.h>
#include "DescisionPipeline.hpp"

#define PADDING 15

/**
 * @brief Returns set of student-names from given map with closest amount of points <= <leqPoints>
 *
 * @param leqPoints point border to search for downwards
 * @param studs map for search
 * @return std::set<std::string>
 */
std::set<std::string> DescisionPipeline::closestLEQPointsStudents(uint8_t leqPoints, std::map<std::string, uint8_t> studs)
{
    if (input->verbose)
        std::cout << "Searching for students with " << to_string(leqPoints) << " points";
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
        if (input->verbose)
            std::cout << "\t- no student found\n";
        return closestLEQPointsStudents(--leqPoints, studs);
    }
    else
    {
        if (input->verbose && !closestStuds.empty())
        {
            std::cout << "\t- found:\n";
            listStrings(closestStuds);
        }

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
    if (input->verbose)
        std::cout << "Searching for students with " << to_string(geqPoints) << " points";
    // Find students with points equal geqPoints
    std::set<std::string> closestStuds;
    for (auto it = studs.begin(); it != studs.end(); ++it)
    {
        if (this->csvMan.getStudent(it->first)->getPoints() == geqPoints)
            closestStuds.insert(it->first);
    }
    // return when Students found, otherwise recur method with greaterPoints + 1 (and geqPoints no overflow to terminate)
    if (closestStuds.empty() && (uint8_t)(geqPoints + 1) >= geqPoints)
    {
        if (input->verbose)
            std::cout << "\t- no student found\n";
        return closestGEQPointsStudents(++geqPoints, studs);
    }
    else
    {
        if (input->verbose && !closestStuds.empty())
        {
            std::cout << "\t- found:\n";
            listStrings(closestStuds);
        }

        return closestStuds;
    }
}
/**
 * @brief Returns maximal priorize value in map for student priorizing.
 *
 * @return uint8_t
 */
uint8_t DescisionPipeline::getMaxPriorizing()
{
    uint8_t max = 0;
    for (auto stud : studPriorizing)
    {
        if (stud.second > max)
            max = stud.second;
    }
    return max;
}
/**
 * @brief Removes all students from map for student priorizing, whose priorize value is less than
 * <priorizeValue>.
 *
 * @param priorizeValue Threshold value
 */
void DescisionPipeline::removeLessPriorizedThen(uint8_t priorizeValue)
{
    std::string discardStudents;
    for (auto stud : studPriorizing)
    {
        if (stud.second < priorizeValue)
        {
            if (input->verbose)
                discardStudents.append(stud.first + ", ");
            studPriorizing.erase(stud.first);
        }
    }
    if (input->verbose)
    {
        if (!discardStudents.empty())
        {
            discardStudents.pop_back(); // remove ' '
            discardStudents.pop_back(); // remove ','
        }
        std::cout << "Discarding students with less than " << to_string(priorizeValue) << " priority\n\t" << discardStudents << "\n";
    }
}
/**
 * @brief Reduces map for student priorizing to students with highest priorize value
 *
 */
void DescisionPipeline::removeLeastPriorized()
{
    uint8_t maxPriorize = getMaxPriorizing();
    if (input->verbose)
        std::cout << "Max priorize-value: " << to_string(maxPriorize) << std::endl;
    ;
    removeLessPriorizedThen(maxPriorize);
}
/**
 * @brief Returns the name of a random student in the priorizing collection
 *
 * @return std::string
 */
std::string DescisionPipeline::getRandomStudent()
{
    srand (time(NULL));
    int randInt = rand() % studPriorizing.size();
    std::map<std::string, uint8_t>::iterator it = studPriorizing.begin();
    for (int i = 0; i < randInt; i++)
    {
        it++;
    }
    return it->first;
}
/**
 * @brief Prints all strings from set to terminal.
 *
 * @param listingSet
 */
void DescisionPipeline::listStrings(std::set<std::string> const &listingSet)
{
    for (auto str : listingSet)
        std::cout << "\t" << str << std::endl;
}
/**
 * @brief Prints all strings of given map to terminal
 *
 * @param listingMap std::map to be printed
 */
void DescisionPipeline::listStrings(std::map<std::string, uint8_t> const &listingMap)
{
    for (auto pair : listingMap)
        std::cout << "\t" << pair.first << std::endl;
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
    if (input->verbose) // verbose output
        std::cout << "preferred points: " << to_string(preferredPoints) << std::endl;
    // Find students to remain
    std::set<std::string> remainingStuds = closestLEQPointsStudents(preferredPoints, this->studPriorizing);
    // Check if students were found that have preferred points
    if (remainingStuds.empty())
    {
        if (input->verbose) // verbose output
        {
            std::cout << "No student with <= " << to_string(preferredPoints) << " points found.\n"
                      << "Searching for students with > " << to_string(preferredPoints) << " points\n";
        }
        remainingStuds = closestGEQPointsStudents(preferredPoints + 1, this->studPriorizing);
    }
    // Discard students from map that are not remaining students
    std::string discardedStuds;
    for (auto it = this->studPriorizing.begin(); it != this->studPriorizing.end();)
    {
        std::string studName = it->first;
        if (remainingStuds.find(studName) == remainingStuds.end()) // if studName not exists in remainStuds
        {
            if (input->verbose)
                discardedStuds.append(it->first + ", ");
            it = this->studPriorizing.erase(it); // iterator points to position after erased element
        }
        else
            it++;
    }
    if (input->verbose)
    {
        if (!discardedStuds.empty())
        {
            discardedStuds.pop_back(); // remove ' '
            discardedStuds.pop_back(); // remove ','
        }
        std::cout << "Discarding all other students of selection\n\t" << discardedStuds << "\n";
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
            if (input->verbose)                                 // verbose output
                std::cout << padTo(studName, PADDING) << "\tis in correct seminar\t- priorize by " << to_string(priorityValue) << std::endl;
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
            if (input->verbose)                                 // verbose output
                std::cout << padTo(studName, PADDING) << "\tseems to be repeater\t- priorize by " << to_string(priorityValue) << std::endl;
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

    if (input->verbose)
    {
        puts("\nStudents of selection that sit furthest in front:");
        listStrings(priorizedStudsInRow);
    }

    // discard all students not in priorizedStudsInRow (= studSelection - priorizedStudsInRow)
    std::set<std::string> discardStuds;
    std::string discardedStr;
    std::set_difference(
        studSelection.begin(), studSelection.end(),
        priorizedStudsInRow.begin(), priorizedStudsInRow.end(),
        std::inserter(discardStuds, discardStuds.begin())); // Inserter for Not-Read-Only Iterator
    for (std::string studName : discardStuds)
    {
        if (input->verbose)
            discardedStr.append(studName + ", ");
        studPriorizing.erase(studName);
    }
    if (input->verbose)
    {
        if (!discardedStr.empty())
        {
            discardedStr.pop_back(); // remove ' '
            discardedStr.pop_back(); // remove ','
        }
        std::cout << "Discarding all other students of selection\n\t" << discardedStr << "\n";
    }
}

/**
 * @brief Construct a new Descision Pipeline:: Descision Pipeline object
 *
 * @param input InputStruct holding the input information
 */
DescisionPipeline::DescisionPipeline(InputStruct const *input) : csvMan(CSVManager(input->csvFile)), input(input)
{
    // verbose output when seating row is considered
    if (input->verbose && input->studSelection.size() > 1)
    {
        std::cout << padTo("seating row", PADDING) << "| "
                  << padTo("   name", PADDING - 1) << "\n"
                  << padTo("", PADDING, '-') << "+" << padTo("", PADDING - 1, '-') << std::endl;

        for (auto elem : input->studSelection)
        {
            for (auto name : elem.second)
            {
                std::cout << padTo(to_string(elem.first), PADDING) << "| "
                          << padTo(name, PADDING - 1) << std::endl;
            }
        }
    }

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
                std::cout << "Student \"" << studName << "\" does not exist." << std::endl;
        }
    }
}

/**
 * @brief Returns pointer to student object. Decides for one student by going through 3 phases of
 * decisions. If there are several students left in the selection at the end, one is chosen at random.
 *
 * @return Student*
 */
Student *DescisionPipeline::decideForStudent()
{
    if (studPriorizing.size() == 0)
    {
        puts("ERROR: no valid selection of students");
        return nullptr; // return nullptr when no students to decide
    }

    // First elimination phase
    if (input->verbose)
        puts("\n----------------- First sorting out --------------------");
    rulePreferredPoints(input->preferredPoints);

    // Prioritization phase
    if (input->semGroup != "")
    {
        if (input->verbose)
            puts("\n----------------- Prioritization phase -----------------");
        rulePriorizeCorrectSemGroup(input->semGroup, input->priorityCorrectSemGroup);
        rulePriorizeRepeaters(input->semGroup, input->priorityRepeater);
    }

    // Second elimination phase
    if (input->verbose)
        puts("\n----------------- Second sorting out -------------------");
    removeLeastPriorized();
    // only if more than 1 row AND more than 1 stud remaining
    if (input->studSelection.size() > 1 && studPriorizing.size() > 1)
        ruleFurthestInFront();

    // Final Decision
    if (input->verbose)
        puts("\n----------------- Final decision phase -----------------");
    if (studPriorizing.size() > 1)
    {
        if (input->verbose)
        {
            puts("At least two students remain:");
            listStrings(studPriorizing);
            puts("--> Random pick of student\n");
        }
        return csvMan.getStudent(getRandomStudent()); // random descision if more than 1 students now
    }
    return csvMan.getStudent(studPriorizing.begin()->first); // return only student in map
}

/**
 * @brief Returns copy of given string padded to given num. When <str> is already bigger than num,
 * nothing happens.
 *
 * @param str string to pad
 * @param num size for string
 * @param paddingChar char to pad with
 * @return std::string
 */
std::string padTo(std::string const &str, const size_t num, const char paddingChar)
{
    std::string padStr = str;
    if (num > padStr.size())
        padStr.insert(padStr.end(), num - padStr.size(), paddingChar);
    return padStr;
}
