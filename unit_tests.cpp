#include <gtest/gtest.h>
#include <filesystem>
#include "Student.hpp"
#include "CSVManager.hpp"
#include "InputStruct.hpp"
#include "DescisionPipeline.hpp"

namespace fs = std::filesystem;

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
        fs::copy("mock_students.csv", "test_students.csv"); // create editable file
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
        fs::copy("mock_students.csv", "test_students.csv"); // create editable file

        std::vector<std::string> studs = {"noExistingOne", "KReide", "MMuster", "JSubjekt", "RSalze"};

        input1 = new InputStruct;
        input1->csvFile = "test_students.csv";
        input1->studSelection = studs;
        input1->verbose = true;
        pipe1 = new DescisionPipeline(input1);

        input2 = new InputStruct;
        input2->csvFile = "test_students.csv";
        input2->studSelection = studs;
        input2->preferredPoints = 2;
        pipe2 = new DescisionPipeline(input2);

        input3 = new InputStruct;
        input3->csvFile = "test_students.csv";
        input3->studSelection = studs;
        input3->preferredPoints = 3;
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

    // Map Count
    int getRemainingSelectionSize(DescisionPipeline *pipe)
    {
        return pipe->studPriorizing.size();
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
TEST_F(DescisionPipelineTest, closestLEQPointsStudentsAssertions)
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
TEST_F(DescisionPipelineTest, closestGEQPointsStudentsAssertions)
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