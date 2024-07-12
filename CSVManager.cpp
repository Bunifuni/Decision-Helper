#include <fstream>
#include <iostream>
#include "CSVManager.hpp"
#include "preprocessing.hpp"

#define DELIMITER ",\n"

#define COLUMN_COUNT 3
#define COLUMN_NAME 0
#define COLUMN_SEMGROUP 1
#define COLUMN_POINTS 2

/**
 * @brief Creates a new Student object from csvLine
 *
 * @param csvLine string in csv-format with student information
 * @param size length of csvLine
 */
Student CSVManager::createStudentFromCSV(char *csvLine, size_t size)
{
    char *funcArg = new char[size];             // stores argument for exception-handling because argument will be altered
    copy(csvLine, csvLine + size + 1, funcArg); // deep copy of function argument (+1 for copying string-end aka \0)

    const char *delimiter = DELIMITER;
    vector<string> tokens = separateLine(csvLine, delimiter);
    try
    {
        return Student(
            tokens.at(COLUMN_NAME),                 // name
            tokens.at(COLUMN_SEMGROUP),             // seminar group
            (uint8_t)stoi(tokens.at(COLUMN_POINTS)) // points
        );
    }
    catch (std::out_of_range &exc)
    {
        cerr << "Error:\t" << exc.what()
             << "\tat creating Student-Obj with:\n\t\""
             << funcArg << "\"" << endl;
        throw;
    }
    catch (std::invalid_argument &excia)
    {
        cerr << "Error:\t" << excia.what()
             << "\tat creating Student-Obj with invalid arg for points:\n\t\""
             << funcArg << "\"" << endl;
        throw;
    }
}

/**
 * @brief Creates string in csv-format from Student obj
 *
 * @param stud Student object for csv-line
 * @return string
 */
std::string CSVManager::createCSVFromStudent(Student stud)
{
    std::string csvColumn[COLUMN_COUNT]; // fill array with column information
    csvColumn[COLUMN_NAME] = stud.getName();
    csvColumn[COLUMN_SEMGROUP] = stud.getSemGroup();
    csvColumn[COLUMN_POINTS] = stud.getPointsAsStr();

    // create string
    std::string str = "";
    for (size_t i = 0; i < COLUMN_COUNT; i++)
    {
        str.append(csvColumn[i]);
        if (i != COLUMN_COUNT - 1)
        {
            str.append(",");
        }
        else
        {
            str.append("\n");
        }
    }

    return str;
}

/**
 * @brief Reads the given CSV-file and returns list of students
 *
 * @param filename name of csv-file
 * @return vector<Student>
 */
std::vector<Student> CSVManager::readCSV(std::string filename)
{
    std::vector<Student> studVec;
    std::ifstream csvStream(filename); // open stream
    std::string line;
    while (getline(csvStream, line))
    {
        studVec.push_back(
            createStudentFromCSV((char *)line.c_str(), line.length()));
    }
    csvStream.close(); // close stream
    return studVec;
}

/**
 * @brief Replaces current list of students with list in csv
 *
 * @param filename name of resulting file
 */
void CSVManager::writeCSV(std::string filename)
{
    std::ofstream csvStream(filename); // open stream
    for (size_t i = 0; i < this->students.size(); i++)
    {
        csvStream << createCSVFromStudent(this->students.at(i));
    }
    csvStream.close(); // close stream
}

/**
 * @brief Changes points of student by 1 point
 * Displays error-message when no student with given name was found.
 *
 * @param name name of student
 * @param doIncrement when true increments by 1; otherwise decrements by 1
 */
void CSVManager::changePoints(string name, bool doIncrement)
{
    Student *stud = getStudent(name);
    if (stud != nullptr)
    {
        if (doIncrement)
        {
            stud->incrementPoints();
        }
        else
        {
            stud->decrementPoints();
        }
        writeCSV(this->filename);
        std::cout << name << " has now " << stud->getPointsAsStr() << " points" << std::endl;
    }
    else
    {
        std::cerr << "Error:\t" << "No existing student with name \"" << name << "\"" << std::endl;
    }
}

CSVManager::CSVManager(std::string filename)
{
    this->filename = filename;
    this->students = readCSV(filename);
}

/**
 * @brief Returns reference to Student-Obj with matching name.
 * Returns nullptr when no matching student found.
 *
 * @param name name of student to search for
 * @return Student*
 */
Student *CSVManager::getStudent(string name)
{
    size_t i = 0;
    while (i < students.size())
    {
        if (this->students.at(i).getName() == name)
        {
            return &students.at(i);
        }
        ++i;
    }
    return nullptr;
}

/**
 * @brief Increments points of student with given name
 *
 * @param name name of student
 */
void CSVManager::incrementPoints(string name)
{
    changePoints(name, true);
}

/**
 * @brief Decrements points of student with given name
 *
 * @param name name of student
 */
void CSVManager::decrementPoints(string name)
{
    changePoints(name, false);
}
