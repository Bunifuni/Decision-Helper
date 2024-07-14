#include <getopt.h>
#include <iostream>
#include <string>
#include <cstring>
#include "preprocessing.hpp"
#include "InputStruct.hpp"

#define STUDENT_SEPARATOR ","
#define SEATINGROW_SEPARATOR ":"

static uint8_t seatingRow_check = 0;

/**
 * @brief Prints the help-text in terminal. When this method is called, the program exits.
 *
 */
void printHelp()
{
    // TODO
    puts("Help");
    exit(1);
}
/**
 * @brief Prints in terminal how to display the help-text.
 *
 */
void printHelpHint()
{
    puts("To display help, use the -h or --help flag.");
}

int preprocessing(int argc, char *argv[], InputStruct *input)
{
    int errorOccured = 0;
    // check for arguments
    if (argc > 1)
    {
        // check for command argument; has to be stored before getopt_long in processOpts is called
        std::string command = argv[1];

        // check for errors
        errorOccured = processOpts(argc, argv, input);

        // distinguish command
        if (decideArgAliases.find(command) != decideArgAliases.end()) // decide for student
            input->state = decision;
        else if (increaseArgAliases.find(command) != increaseArgAliases.end()) // increment students points
            input->state = increment;
        else if (decreaseArgAliases.find(command) != decreaseArgAliases.end()) // decrement students points
            input->state = decrement;
        else
        {
            std::cout << "unknown command: \"" << command << "\"\n";
            errorOccured = -1;
        }
    }
    else
    {
        puts("missing command");
        errorOccured = -1;
    }
    // print hint for help when errors occured
    if (errorOccured == -1)
    {
        printHelpHint();
        return -1;
    }
    return 0;
}

/**
 * @brief Processes options of program-call. Returns 0 when succeed with no complication.
 * Returns -1 otherwise. Changes properties of InputStruct based on options.
 *
 * @param argc argument count
 * @param argv argument vector
 * @param input InputStruct to encapsulate the result
 * @return int
 */
int processOpts(int argc, char *argv[], InputStruct *input)
{
    const char *const short_opts = "f:g:s:hrv";
    const option long_opts[] = {
        {"file", required_argument, nullptr, 'f'},
        {"group", required_argument, nullptr, 'g'},
        {"semgroup", required_argument, nullptr, 'g'},
        {"semGroup", required_argument, nullptr, 'g'},
        {"seminar", required_argument, nullptr, 'g'},
        {"selection", required_argument, nullptr, 's'},
        {"students", required_argument, nullptr, 's'},
        // flags
        {"help", no_argument, nullptr, 'h'},
        {"row", no_argument, nullptr, 'r'},
        {"verbose", no_argument, nullptr, 'v'},
        {0, 0, 0, 0}};

    int c;
    char *selectionStr = nullptr;
    while (true)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, short_opts, long_opts, &option_index);

        // detect end of options
        if (c == -1)
            break;

        // distinguish options
        switch (c)
        {
        case 0:
            break;

        case 'f':
            // printf("option -f with value `%s'\n", optarg);
            input->csvFile = optarg;
            break;

        case 'g':
            // printf("option -g with value `%s'\n", optarg);
            input->semGroup = optarg;
            break;

        case 's':
            // printf("option -s with value `%s'\n", optarg);
            selectionStr = optarg;
            break;

        case 'h':
            // puts("option -h\n");
            printHelp();
            break;

        case 'r':
            // puts("option -r\n");
            seatingRow_check = (uint8_t)1;
            break;
        case 'v':
            // puts("option -v\n");
            input->verbose = true;
            break;

        case '?':
            break;

        default:
            abort();
        }
    }

    // check if selection is empty
    if (selectionStr == nullptr)
    {
        puts("Selection of students is missing.");
        return -1;
    }

    input->studSelection = processSelectionStr(selectionStr); // process selection of students

    // check if selection is valid
    if (input->studSelection.empty())
    {
        std::cout << "Selection argument is not valid. It has to be \n 1 student:\t<studentName>";
        if (seatingRow_check)
            std::cout << ":<seatingRow>";
        std::cout << "\n>1 students:\t<student1Name>";
        if (seatingRow_check)
            std::cout << ":<seatingRow>";
        std::cout << ",<student2Name>";
        if (seatingRow_check)
            std::cout << ":<seatingRow>";
        std::cout << ",...\n";
        return -1;
    }

    return 0;
}

/**
 * @brief Splits the words in the vector to <string, int> by the delimiter. Returns vector with splitted words.
 *
 * @param unsplitVector unsplitted vector
 * @param delimiter delimiter to split by
 * @return std::vector<std::string, int>
 */
std::vector<std::pair<std::string, int>> splitElements(std::vector<std::string> unsplitVector, const char *delimiter)
{
    std::vector<std::pair<std::string, int>> splitVector;
    for (std::string unsplitWord : unsplitVector)
    {
        int delimiterPos = unsplitWord.find(delimiter); // get delimiter position
        splitVector.push_back({
            unsplitWord.substr(0, delimiterPos),                                                     // word before delimiter
            std::stoi(unsplitWord.substr(delimiterPos + 1, unsplitWord.size() - (delimiterPos + 1))) // word after delimiter
        });
    }
    return splitVector;
}

/**
 * @brief Processes the given string of students. Returns map with students as set per row.
 *
 * @param selectionStr
 * @return std::map<int, std::set<std::string>>
 */
std::map<int, std::set<std::string>> processSelectionStr(char *selectionStr)
{
    std::map<int, std::set<std::string>> selectionMap;
    std::set<std::string> studsInRow;
    std::vector<std::string> splitStuds = separateLine(selectionStr, STUDENT_SEPARATOR);

    // check if seating row has to be considered
    if (seatingRow_check)
    {
        auto splittedVector = splitElements(splitStuds, SEATINGROW_SEPARATOR); // split names from seating row
        int row = 0;
        // insert students row by row to map
        while (splittedVector.empty() == false)
        {
            studsInRow.clear(); // clear set
            // collect all students with current row in set
            for (auto it = splittedVector.begin(); it < splittedVector.end();)
            {
                if (it->second == row)
                {
                    studsInRow.insert(it->first);
                    it = splittedVector.erase(it); // erase element
                }
                else
                {
                    it++;
                }
            }
            selectionMap.insert({row, studsInRow});
            row++;
        }
    }
    else
    {
        // seating row not considered
        for (std::string studName : splitStuds)
        {
            studsInRow.insert(studName);
        }
        selectionMap.insert({0, studsInRow});
    }

    // TODO verbose
    for (auto elem : selectionMap)
    {
        for (auto name : elem.second)
        {
            std::cout << elem.first << "\t" << name << std::endl;
        }
    }

    return selectionMap;
}

/**
 * @brief Separates given line by given delimiter and returns list of separated strings
 *
 * @param line string to separate
 * @param delimiter delimiter to separate by
 * @return vector<string>
 */
std::vector<std::string> separateLine(char *line, const char *delimiter)
{
    std::vector<std::string> words;        // vector to collect strings
    char *token = strtok(line, delimiter); // char* until delimiter
    while (token != nullptr)
    {
        std::string str = token; // string instead of char* for string methods (string cannot be nullptr)
        if (str.back() == '\n')  // does str end with newline(LF)?
        {
            str.pop_back(); // remove newline
        }
        words.push_back(str);               // add word to list
        token = strtok(nullptr, delimiter); // next word
    }
    return words;
}
