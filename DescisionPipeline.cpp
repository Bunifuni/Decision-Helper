#include <iostream>
#include "DescisionPipeline.hpp"

DescisionPipeline::DescisionPipeline(InputStruct const *input) : csvMan(CSVManager(input->csvFile)), input(input)
{
    // create <map> from students
    for (std::string studName : input->studSelection)
    {
        // does given student exist?
        if (csvMan.getStudent(studName) != nullptr)
            this->studPriorizing.insert({studName, 0});
        else if (input->verbose)
            std::cout << "Student \"" << studName << "\" existiert nicht." << std::endl;
    }
}