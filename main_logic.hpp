#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "algorithms.hpp"
#include "casts.hpp"
#include "csvrow.hpp"
#include "server_pool.hpp"

class MainLogic {
public:
    MainLogic(std::string input_file, int resources_number) : queries(std::move(input_file)), serverPool(resources_number) {}

    void Run(const std::string& problem_type, const std::string& algorithm, const std::string& heuristic) {
        if (problem_type == "dynamicplacement") {
            RunDynamicPlacement(algorithm, heuristic);
        } else {
            std::cout << "No such problem type: " << problem_type << std::endl;
            exit(0);
        }
    }

private:
    void RunDynamicPlacement(const std::string& algorithm, const std::string& heuristic) {
        /// CSV must be organized as follows:
        /// id,start_or_end,time,resource_1,...,resource_i
        /// where i is number of resources

        std::ifstream file(queries);
        CSVRow row;

        while (file >> row) {
            int resources_number = serverPool.GetResourcesNumber();

            if (row.size() < resources_number + 3) {
                std::cout << "CSV file is incorrect" << std::endl << std::endl;
                std::cout << "CSV must be organized as follows:" << std::endl;
                std::cout << "id,start_or_end,time,resource_1,...,resource_i" << std::endl;
                std::cout << "where i is number of resources" << std::endl;
                exit(0);
            }

            int id = string2int(row[0]);
            std::string_view start_or_end = row[1];
            long double time = string2longdouble(row[2]);
            std::vector<long double> resources(resources_number);
            for (int i = 1; i <= resources_number; ++i) {
                resources[i - 1] = string2longdouble(row[i + 2]);
            }

            if (start_or_end == "end") {
                serverPool.DeleteById(id);
                continue;
            }

            int server_id;
            if (algorithm == "firstfit") {
                server_id = Algorithms::FirstFit(serverPool.GetServers(), resources);
            } else if (algorithm == "bestfit") {
                server_id = Algorithms::BestFit(heuristic, serverPool.GetServers(), resources);
            } else {
                std::cout << "For problem type dynamic placement" << std::endl;
                std::cout << "No such algorithm: " << algorithm << std::endl;
                exit(0);
            }

            if (server_id == -1) {
                serverPool.AddServerAndPlace(id, resources);
            } else {
                serverPool.Place(server_id, id, resources);
            }
        }
        serverPool.PrintServersSummaryToFile("servers_load.csv");
    }

    std::string queries;
    ServerPool serverPool;
};
