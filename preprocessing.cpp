#include <iostream>
#include "preprocessing.hpp"
#include "InputStruct.hpp"

using namespace std;

int preprocesssing(int argc, char *argv[])
{
    processingResult result = unhandled;
    // check for arguments
    if (argc > 1)
    {
        // check for command argument
        char *command = argv[1];
        if (command == "cmd1")
            result = decision;
        else if (increaseArgs.find(command) != increaseArgs.end())
            result = increment;
        else if (decreaseArgs.find(command) != decreaseArgs.end())
            result = decrement;
        else
        {
            printf("unknown command: \"%s\"\n", command);
            result = error;
        }
    }
    else
    {
        printf("missing command\n");
        result = error;
    }
    return result;
}
