#include <iostream>
#include "preprocessing.hpp"
#include "DescisionPipeline.hpp"

int main(int argc, char *argv[])
{
    InputStruct input;
    if (preprocessing(argc, argv, &input) == -1)
        exit(-1);
    DescisionPipeline decider(&input);
    Student *chosenOne;
    switch (input.state)
    {
    case decision:
        chosenOne = decider.decideForStudent();
        if (chosenOne)
            std::cout << "The chosen student is: \t" << chosenOne->getName() << std::endl;
        break;
    case increment:
        decider.incrementPointsOfSelection();
        break;
    case decrement:
        decider.decrementPointsOfSelection();
        break;
    default:
        puts("ERROR: command unhandled");
    }

    return 0;
}
