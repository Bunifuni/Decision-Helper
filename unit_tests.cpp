#include <gtest/gtest.h>
#include <filesystem>
#include <random>
#include "Student.hpp"
#include "CSVManager.hpp"
#include "InputStruct.hpp"
#include "DescisionPipeline.hpp"

namespace fs = std::filesystem;
const char *mockfile = "mock_students.csv";

/******************************* Fixtures ******************************************/
class StudentTest : public testing::Test
{
protected:
    Student *stud1;
    Student *stud2;
    Student *stud3;
    Student *stud4;

    StudentTest()
    {
        stud1 = new Student("MYstud1", "class1", (uint8_t)0);
        stud2 = new Student("MYstud2", "class1", (uint8_t)1);
        stud3 = new Student("MYstud3", "class2", (uint8_t)2);
        stud4 = new Student("MYstud4", "class2", (uint8_t)4);
    }
};

class CSVManagerTest : public testing::Test
{
protected:
    CSVManager *csvMan;

    CSVManagerTest()
    {
        fs::copy(mockfile, "test_students.csv"); // create editable file
        csvMan = new CSVManager("test_students.csv");
    }

    ~CSVManagerTest()
    {
        fs::remove("test_students.csv"); // remove file for testing
    }
};

class DescisionPipelineTest : public testing::Test
{
protected:
    InputStruct *input1;
    InputStruct *input2;
    InputStruct *input3;
    InputStruct *input4;
    DescisionPipeline *pipe1;
    DescisionPipeline *pipe2;
    DescisionPipeline *pipe3;
    DescisionPipeline *pipe4;

    DescisionPipelineTest()
    {
        fs::copy(mockfile, "test_students.csv"); // create editable file

        std::map<int, std::set<std::string>> studs = {
            {1, {"MMuster", "RSalze"}},
            {3, {"noExistingOne", "KReide", "JSubjekt"}}};

        input1 = new InputStruct;
        input1->csvFile = "test_students.csv";
        input1->studSelection = studs;
        input1->verbose = true;
        input1->semGroup = "22INB-2";
        pipe1 = new DescisionPipeline(input1);

        input2 = new InputStruct;
        input2->csvFile = "test_students.csv";
        input2->studSelection = studs;
        input2->preferredPoints = 2;
        input2->semGroup = "21INB-1";
        pipe2 = new DescisionPipeline(input2);

        input3 = new InputStruct;
        input3->csvFile = "test_students.csv";
        input3->studSelection = studs;
        input3->preferredPoints = 3;
        input3->semGroup = "23INB-3";
        pipe3 = new DescisionPipeline(input3);

        input4 = new InputStruct;
        input4->csvFile = "test_students.csv";
        input4->studSelection = studs;
        input4->preferredPoints = 4;
        pipe4 = new DescisionPipeline(input4);
    }

    ~DescisionPipelineTest()
    {
        fs::remove("test_students.csv"); // remove file for testing
    }

    // Wrapper functions
    void rulePreferredPoints(DescisionPipeline *pipe)
    {
        pipe->rulePreferredPoints(pipe->input->preferredPoints);
    }
    std::set<string> closestLEQPointsStudents(uint8_t leqPoints, DescisionPipeline *pipe)
    {
        return pipe->closestLEQPointsStudents(leqPoints, pipe->studPriorizing);
    }
    std::set<string> closestGEQPointsStudents(uint8_t geqPoints, DescisionPipeline *pipe)
    {
        return pipe->closestGEQPointsStudents(geqPoints, pipe->studPriorizing);
    }
    void rulePriorizeCorrectSemGroup(std::string semGroup, uint8_t priorizeValue, DescisionPipeline *pipe)
    {
        pipe->rulePriorizeCorrectSemGroup(semGroup, priorizeValue);
    }
    void rulePriorizeRepeaters(std::string semGroup, uint8_t priorizeValue, DescisionPipeline *pipe)
    {
        pipe->rulePriorizeRepeaters(semGroup, priorizeValue);
    }
    void ruleFurthestInFront(DescisionPipeline *pipe)
    {
        pipe->ruleFurthestInFront();
    }
    void removeLeastPriorized(DescisionPipeline *pipe)
    {
        pipe->removeLeastPriorized();
    }

    // Map Count
    int getRemainingSelectionSize(DescisionPipeline *pipe)
    {
        return pipe->studPriorizing.size();
    }
    // See priorize value
    uint8_t getPriorizing(std::string studName, DescisionPipeline *pipe)
    {
        return pipe->studPriorizing.at(studName);
    }
    // set priorizing map
    void setPriorizingMap(std::map<std::string, uint8_t> priorizeMap, DescisionPipeline *pipe)
    {
        pipe->studPriorizing = priorizeMap;
    }
    // get priorizing map
    std::map<std::string, uint8_t> getPriorizingMap(DescisionPipeline *pipe)
    {
        return pipe->studPriorizing;
    }
};
/***********************************************************************************/

/* --- Testing class Student --- */
// Testing basic get-methods
TEST_F(StudentTest, GetAssertions)
{
    // Test getName
    ASSERT_STREQ((stud1->getName()).c_str(), "MYstud1");
    ASSERT_STREQ((stud2->getName()).c_str(), "MYstud2");
    ASSERT_STREQ((stud3->getName()).c_str(), "MYstud3");
    ASSERT_STREQ((stud4->getName()).c_str(), "MYstud4");
    // Test getSemGroup
    ASSERT_STREQ((stud1->getSemGroup()).c_str(), "class1");
    ASSERT_STREQ((stud2->getSemGroup()).c_str(), "class1");
    ASSERT_STREQ((stud3->getSemGroup()).c_str(), "class2");
    ASSERT_STREQ((stud4->getSemGroup()).c_str(), "class2");
    // Test getPoints
    ASSERT_EQ(stud1->getPoints(), 0);
    ASSERT_EQ(stud2->getPoints(), 1);
    ASSERT_EQ(stud3->getPoints(), 2);
    ASSERT_EQ(stud4->getPoints(), 4);
    // Test getPointsAsStr
    ASSERT_STREQ((stud1->getPointsAsStr()).c_str(), "0");
    ASSERT_STREQ((stud2->getPointsAsStr()).c_str(), "1");
    ASSERT_STREQ((stud3->getPointsAsStr()).c_str(), "2");
    ASSERT_STREQ((stud4->getPointsAsStr()).c_str(), "4");
}
// Testing increment-method
TEST_F(StudentTest, IncrementAssertions)
{
    // Old Points
    int p1 = stud1->getPoints();
    int p2 = stud2->getPoints();
    int p3 = stud3->getPoints();
    int p4 = stud4->getPoints();
    // New points
    stud1->incrementPoints();
    stud2->incrementPoints();
    stud3->incrementPoints();
    stud4->incrementPoints();

    ASSERT_EQ(stud1->getPoints(), p1 + 1);
    ASSERT_EQ(stud2->getPoints(), p2 + 1);
    ASSERT_EQ(stud3->getPoints(), p3 + 1);
    ASSERT_EQ(stud4->getPoints(), p4 + 1);
}
// Testing decrement-method
TEST_F(StudentTest, DecrementAssertions)
{
    // Old Points
    int p1 = stud1->getPoints();
    int p2 = stud2->getPoints();
    int p3 = stud3->getPoints();
    int p4 = stud4->getPoints();
    // New Points
    stud1->decrementPoints();
    stud2->decrementPoints();
    stud3->decrementPoints();
    stud4->decrementPoints();

    ASSERT_EQ(stud1->getPoints(), max(p1 - 1, 0));
    ASSERT_EQ(stud2->getPoints(), max(p2 - 1, 0));
    ASSERT_EQ(stud3->getPoints(), max(p3 - 1, 0));
    ASSERT_EQ(stud4->getPoints(), max(p4 - 1, 0));
}

/* --- Testing class CSVManager --- */
// Testing getStudent-method
TEST_F(CSVManagerTest, GetStudentAssertions)
{
    Student *stud1 = csvMan->getStudent("MMuster");
    Student *stud2 = csvMan->getStudent("KReide");
    Student *stud3 = csvMan->getStudent("noExisting");
    ASSERT_NE(stud1, nullptr); // stud1 points to existing obj
    ASSERT_NE(stud2, nullptr); // stud2 points to existing obj
    ASSERT_EQ(stud3, nullptr); // stud3 points to nullptr (no stud with given name)
}

/* --- Testing class DescisionPipeline --- */
// Testing closestLEQPointsStudents
TEST_F(DescisionPipelineTest, ClosestLEQPointsStudentsAssertions)
{
    // selection = {"noExistingOne" -> NaN, "KReide" -> 4, "MMuster" -> 1, "JSubjekt" -> 1, "RSalze" -> 1}
    // preferredPoints = 0
    ASSERT_EQ(closestLEQPointsStudents(0, pipe1).size(), 0); // empty
    // preferredPoints = 2
    ASSERT_EQ(closestLEQPointsStudents(2, pipe2).size(), 3); // MMuster, JSubjekt, RSalze
    // preferredPoints = 3
    ASSERT_EQ(closestLEQPointsStudents(3, pipe3).size(), 3); // MMuster, JSubjekt, RSalze
    // preferredPoints = 4
    ASSERT_EQ(closestLEQPointsStudents(4, pipe4).size(), 1); // Kreide
}
// Testing closestLEQPointsStudents
TEST_F(DescisionPipelineTest, ClosestGEQPointsStudentsAssertions)
{
    // selection = {"noExistingOne" -> NaN, "KReide" -> 4, "MMuster" -> 1, "JSubjekt" -> 1, "RSalze" -> 1}
    // preferredPoints = 0
    ASSERT_EQ(closestGEQPointsStudents(0, pipe1).size(), 3); // MMuster, JSubjekt, RSalze
    // preferredPoints = 2
    ASSERT_EQ(closestGEQPointsStudents(2, pipe1).size(), 1); // KReide
    // preferredPoints = 3
    ASSERT_EQ(closestGEQPointsStudents(3, pipe1).size(), 1); // KReide
    // preferredPoints = 4
    ASSERT_EQ(closestGEQPointsStudents(4, pipe1).size(), 1); // KReide
    // preferredPoints = 5
    ASSERT_EQ(closestGEQPointsStudents(5, pipe1).size(), 0); // empty
}
// Testing rulePreferredPoints
TEST_F(DescisionPipelineTest, RulePreferredPointsAssertions)
{
    // selection = {"noExistingOne" -> NaN, "KReide" -> 4, "MMuster" -> 1, "JSubjekt" -> 1, "RSalze" -> 1}

    // preferredPoints = 0
    rulePreferredPoints(pipe1);
    ASSERT_EQ(getRemainingSelectionSize(pipe1), 3);
    // preferredPoints = 2
    rulePreferredPoints(pipe2);
    ASSERT_EQ(getRemainingSelectionSize(pipe2), 3);
    // preferredPoints = 3
    rulePreferredPoints(pipe3);
    ASSERT_EQ(getRemainingSelectionSize(pipe3), 3);
    // preferredPoints = 4
    rulePreferredPoints(pipe4);
    ASSERT_EQ(getRemainingSelectionSize(pipe4), 1);
}
// Testing rulePriorizeCorrectSemGroup
TEST_F(DescisionPipelineTest, RulePriorizeCorrectSemGroupAssertions)
{
    // selection = {"noExistingOne" -> NaN, "KReide" -> 22INB-1; "MMuster" -> 21INB-1; "JSubjekt" -> 22INB-2; "RSalze" -> 22INB-2}
    int priorizeValue;
    // Test on semGroup "22INB-2" with priorizeValue 2
    priorizeValue = 2;
    rulePriorizeCorrectSemGroup("22INB-2", priorizeValue, pipe1);
    ASSERT_EQ(getPriorizing("KReide", pipe1), 0);               // not in semGroup
    ASSERT_EQ(getPriorizing("MMuster", pipe1), 0);              // not in semGroup
    ASSERT_EQ(getPriorizing("JSubjekt", pipe1), priorizeValue); // in semGroup
    ASSERT_EQ(getPriorizing("RSalze", pipe1), priorizeValue);   // in semGroup

    // Test on semGroup "22INB-2" with priorizeValue 5
    priorizeValue = 5;
    rulePriorizeCorrectSemGroup("22INB-2", priorizeValue, pipe2);
    ASSERT_EQ(getPriorizing("KReide", pipe2), 0);               // not in semGroup
    ASSERT_EQ(getPriorizing("MMuster", pipe2), 0);              // in semGroup
    ASSERT_EQ(getPriorizing("JSubjekt", pipe2), priorizeValue); // not in semGroup
    ASSERT_EQ(getPriorizing("RSalze", pipe2), priorizeValue);   // in semGroup

    // Test on semGroup "21INB-1" with priorizeValue 3
    priorizeValue = 3;
    rulePriorizeCorrectSemGroup("21INB-1", priorizeValue, pipe3);
    ASSERT_EQ(getPriorizing("KReide", pipe3), 0);              // not in semGroup
    ASSERT_EQ(getPriorizing("MMuster", pipe3), priorizeValue); // in semGroup
    ASSERT_EQ(getPriorizing("JSubjekt", pipe3), 0);            // not in semGroup
    ASSERT_EQ(getPriorizing("RSalze", pipe3), 0);              // not in semGroup

    // Test on semGroup "23INB-3" with priorizeValue 3
    priorizeValue = 3;
    rulePriorizeCorrectSemGroup("23INB-3", priorizeValue, pipe4);
    ASSERT_EQ(getPriorizing("KReide", pipe4), 0);   // not in semGroup
    ASSERT_EQ(getPriorizing("MMuster", pipe4), 0);  // not in semGroup
    ASSERT_EQ(getPriorizing("JSubjekt", pipe4), 0); // not in semGroup
    ASSERT_EQ(getPriorizing("RSalze", pipe4), 0);   // not in semGroup
}
// Testing rulePriorizeRepeaters
TEST_F(DescisionPipelineTest, RulePriorizeRepeatersAssertions)
{
    // selection = {"noExistingOne" -> NaN, "KReide" -> 22INB-1; "MMuster" -> 21INB-1; "JSubjekt" -> 22INB-2; "RSalze" -> 22INB-2}
    int priorizeValue;
    // Test on semGroup "22INB-2" with priorizeValue 2
    priorizeValue = 2;
    rulePriorizeRepeaters("22INB-2", priorizeValue, pipe1);
    ASSERT_EQ(getPriorizing("KReide", pipe1), 0);              // no repeater
    ASSERT_EQ(getPriorizing("MMuster", pipe1), priorizeValue); // repeater
    ASSERT_EQ(getPriorizing("JSubjekt", pipe1), 0);            // no repeater
    ASSERT_EQ(getPriorizing("RSalze", pipe1), 0);              // no repeater

    // Test on semGroup "22INB-2" with priorizeValue 5
    int priorizeValue2 = 5;
    rulePriorizeRepeaters("22INB-2", priorizeValue2, pipe1);
    ASSERT_EQ(getPriorizing("KReide", pipe1), 0);                               // no repeater
    ASSERT_EQ(getPriorizing("MMuster", pipe1), priorizeValue + priorizeValue2); // repeater
    ASSERT_EQ(getPriorizing("JSubjekt", pipe1), 0);                             // no repeater
    ASSERT_EQ(getPriorizing("RSalze", pipe1), 0);                               // no repeater

    // Test on semGroup "23INB-1" with priorizeValue 3
    priorizeValue = 3;
    rulePriorizeRepeaters("23INB-1", priorizeValue, pipe3);
    ASSERT_EQ(getPriorizing("KReide", pipe3), priorizeValue);   // repeater
    ASSERT_EQ(getPriorizing("MMuster", pipe3), priorizeValue);  // repeater
    ASSERT_EQ(getPriorizing("JSubjekt", pipe3), priorizeValue); // repeater
    ASSERT_EQ(getPriorizing("RSalze", pipe3), priorizeValue);   // repeater
}
// Testing ruleFurthestInFront
TEST_F(DescisionPipelineTest, RuleFurthestInFrontAssertions)
{
    DescisionPipeline *pipe;

    input1->studSelection = {
        {1, {"MMuster", "RSalze"}},
        {3, {"noExistingOne", "KReide", "JSubjekt"}}};
    pipe = new DescisionPipeline(input1);
    ruleFurthestInFront(pipe);
    ASSERT_EQ(getRemainingSelectionSize(pipe), 2); // = size_of {"MMuster", "RSalze"}

    input1->studSelection = {
        {1, {"MMuster", "RSalze", "KReide"}},
        {3, {"noExistingOne", "JSubjekt"}}};
    pipe = new DescisionPipeline(input1);
    ruleFurthestInFront(pipe);
    ASSERT_EQ(getRemainingSelectionSize(pipe), 3); // = size_of {"MMuster", "RSalze", "KReide"}

    input1->studSelection = {
        {0, {}},
        {1, {"MMuster", "RSalze", "KReide"}},
        {3, {"noExistingOne", "JSubjekt"}}};
    pipe = new DescisionPipeline(input1);
    ruleFurthestInFront(pipe);
    ASSERT_EQ(getRemainingSelectionSize(pipe), 3); // = size_of {"MMuster", "RSalze", "KReide"}

    input1->studSelection = {
        {0, {"DoesNotExist"}},
        {1, {"MMuster", "RSalze", "KReide"}},
        {3, {"noExistingOne", "JSubjekt"}}};
    pipe = new DescisionPipeline(input1);
    ruleFurthestInFront(pipe);
    ASSERT_EQ(getRemainingSelectionSize(pipe), 3); // = size_of {"MMuster", "RSalze", "KReide"}

    input1->studSelection = {
        {0, {"DoesNotExist", "JSubjekt"}},
        {1, {"MMuster", "RSalze", "KReide"}},
        {3, {"noExistingOne"}}};
    pipe = new DescisionPipeline(input1);
    ruleFurthestInFront(pipe);
    ASSERT_EQ(getRemainingSelectionSize(pipe), 1); // = size_of {"JSubjekt"}
}
// Testing removeLeastPriorized
TEST_F(DescisionPipelineTest, RemoveLeastPriorizedAssertions)
{
    uint8_t MAX_VALUE = (uint8_t)std::rand();
    std::map<std::string, uint8_t> testMap;
    std::map<std::string, uint8_t> checkMap;
    // fill testMap
    for (int i = 0; i < 100; i++)
    {
        uint8_t randInt = (uint8_t)(std::rand() % MAX_VALUE + 1);
        testMap.insert({std::to_string(i), randInt});
        if (randInt == MAX_VALUE) // if MAX_VALUE fill also in checkMap for later assertion
            checkMap.insert({std::to_string(i), randInt});
    }
    setPriorizingMap(testMap, pipe1);

    // execute and assertion
    removeLeastPriorized(pipe1);
    ASSERT_EQ(getRemainingSelectionSize(pipe1), checkMap.size());
    ASSERT_NE(getRemainingSelectionSize(pipe1), 0);
    ASSERT_EQ(getPriorizingMap(pipe1), checkMap);
}
// Testing decideForStudent
TEST_F(DescisionPipelineTest, DecideForStudentAssertions)
{
    std::string studName;
    input1->preferredPoints = 0;         // prefer no points
    input1->priorityCorrectSemGroup = 2; // prefer correct sem
    input1->priorityRepeater = 1;        // prefer repeaters not as much as correct sem
    input1->semGroup = "";               // no sem
    // seating row important
    input1->studSelection = {
        {0, {"KReide", "FMeier"}},
        {1, {"JSubjekt", "RSalze"}},
        {2, {"CSchmidt", "MMuster"}}};
    pipe1 = new DescisionPipeline(input1);
    studName = pipe1->decideForStudent()->getName();
    ASSERT_EQ(studName, "CSchmidt");

    input1->preferredPoints = 1;         // prefer 1 point
    input1->priorityCorrectSemGroup = 2; // prefer correct sem
    input1->priorityRepeater = 1;        // prefer repeaters not as much as correct sem
    input1->semGroup = "22INB-2";
    // seating row important
    input1->studSelection = {
        {0, {"KReide", "FMeier"}},
        {1, {"JSubjekt", "RSalze"}},
        {2, {"MMuster"}}};
    pipe1 = new DescisionPipeline(input1);
    studName = pipe1->decideForStudent()->getName();
    ASSERT_EQ((studName == "JSubjekt") || (studName == "RSalze"), true);

    input1->preferredPoints = 1;         // prefer 1 points
    input1->priorityCorrectSemGroup = 2; // prefer correct sem
    input1->priorityRepeater = 0;        // prefer repeaters as much as incorrect sem
    input1->semGroup = "22INB-2";
    // seating row important
    input1->studSelection = {
        {0, {"KReide", "FMeier"}},
        {1, {"JSubjekt", "RSalze"}},
        {2, {"MMuster"}}};
    pipe1 = new DescisionPipeline(input1);
    studName = pipe1->decideForStudent()->getName();
    ASSERT_EQ((studName == "JSubjekt") || (studName == "RSalze"), true);

    input1->preferredPoints = 1;         // prefer 1 points
    input1->priorityCorrectSemGroup = 2; // prefer correct sem
    input1->priorityRepeater = 1;        // prefer repeaters more than incorrect sem
    input1->semGroup = "22INB-1";
    // seating row important
    input1->studSelection = {
        {0, {"KReide", "FMeier"}},
        {1, {"JSubjekt", "RSalze"}},
        {2, {"MMuster"}}};
    pipe1 = new DescisionPipeline(input1);
    studName = pipe1->decideForStudent()->getName();
    ASSERT_EQ(studName, "MMuster");
}
