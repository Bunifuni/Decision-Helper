#include <gtest/gtest.h>
#include "Student.hpp"

// Fixtures
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