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
#include "server.hpp"

class MainLogic {
public:
    MainLogic(std::string input_file, int resources_number) : queries(std::move(input_file)), server(resources_number) {}

    void Run(std::string algorithm, std::string heuristic) {
        if (algorithm == "dynamicplacement") {
            RunDynamicPlacement(std::move(heuristic));
        }
    }

private:
    void RunDynamicPlacement(std::string heuristic) {
        /// CSV must be organized as follows:
        /// id,isStart,resource_1,...,resource_i
        /// where i is number of resources

        std::ifstream file(queries);
        CSVRow row;

        while (file >> row) {
            int resources_number = server.GetResourcesNumber();

            if (row.size() < resources_number + 2) {
                std::cout << "CSV file is incorrect" << std::endl << std::endl;
                std::cout << "CSV must be organized as follows:" << std::endl;
                std::cout << "id,isStart,resource_1,...,resource_i" << std::endl;
                std::cout << "where i is number of resources" << std::endl;
                exit(0);
            }

            int id = string2int(row[0]);
            int isStart = string2int(row[1]);
            std::vector<long double> resources(resources_number);
            for (int i = 1; i <= resources_number; ++i) {
                resources[i - 1] = string2longdouble(row[i + 1]);
            }

            if (isStart == 0) {
                server.DeleteById(id);
                continue;
            }

            int server_id = Algorithms::DynamicPlacement(heuristic, server.GetServers(), resources);

            if (server_id == -1) {
                server.AddServerAndPlace(id, resources);
            } else {
                server.Place(server_id, id, resources);
            }
        }
        server.PrintServersSummaryToFile("servers_load.csv");
    }

    std::string queries;
    Server server;
};
