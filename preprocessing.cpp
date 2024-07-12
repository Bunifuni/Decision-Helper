#include <getopt.h>
#include <iostream>
#include <string>
#include "preprocessing.hpp"
#include "InputStruct.hpp"

static uint8_t seatingRow_check = 0;

void printHelp()
{
    // TODO
    puts("Help");
    exit(1);
}

void printHelpHint()
{
    puts("To display help, use the -h or --help flag.");
}

int preprocesssing(int argc, char *argv[])
{
    processingResult result = unhandled;
    // check for arguments
    if (argc > 1)
    {
        // check for command argument
        std::string command = argv[1];
        processOpts(argc, argv);

        if (command == "cmd1")
            result = decision;
        else if (increaseArgs.find(command) != increaseArgs.end())
            result = increment;
        else if (decreaseArgs.find(command) != decreaseArgs.end())
            result = decrement;
        else
        {
            std::cout << "unknown command: \"" << command << "\"\n";
            result = error;
        }
    }
    else
    {
        puts("missing command");
        result = error;
    }
    // print hint for help when errors occured
    if (result == error)
    {
        printHelpHint();
    }
    return result;
}

InputStruct *processOpts(int argc, char *argv[])
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

    InputStruct *input = new InputStruct();

    int c;
    std::string selectionStr;
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
    if (selectionStr.empty())
    {
        puts("Selection of students is missing.");
        return nullptr;
    }

    input->studSelection = processSelectionStr(selectionStr);

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
        return nullptr;
    }

    return input;
}

std::map<int, std::set<std::string>> processSelectionStr(std::string selectionStr)
{
    return std::map<int, std::set<std::string>>();
}
