#include <iostream>
#include <string>

#include "main_logic.hpp"

const int ARGUMENTS_COUNT = 5;
const std::string ARGUMENTS[ARGUMENTS_COUNT] = {"input files pattern", "problem type", "algorithm", "heuristic", "number of resources"};

int main(int argc, char *argv[]) {
    if (argc - 1 < ARGUMENTS_COUNT) {
        std::cout << "Wrong number of command line arguments" << std::endl;
        std::cout << "Must be " << ARGUMENTS_COUNT << " arguments:" << std::endl;
        for (int i = 0; i < ARGUMENTS_COUNT; ++i) {
            std::cout << i + 1 << ") " << ARGUMENTS[i] << std::endl;
        }
        return 0;
    }

    std::string pattern = argv[1];
    std::string problem_type = argv[2];
    std::string algorithm = argv[3];
    std::string heuristic = argv[4];
    std::string resources_number_str = argv[5];

    std::string folder = problem_type + "_" + algorithm + "_" + heuristic;

    int resources_number = string2int(resources_number_str);

    for (int i = 1; i <= 100; ++i) {
        std::string input_file = pattern + int2string(i) + ".csv";
        std::cout << input_file << std::endl;
        MainLogic mainLogic(input_file, resources_number, i, folder);
        mainLogic.Run(problem_type, algorithm, heuristic);
    }

    return 0;
}
