#pragma once

#include <iostream>
#include <vector>

class Algorithms {
public:
    static int DynamicPlacement(const std::string& heuristic, const std::vector<std::vector<long double>>& servers, const std::vector<long double>& resources) {
        if (heuristic == "firstfit") {
            for (int i = 0; i < servers.size(); ++i) {
                if (Fits(servers[i], resources)) {
                    return i;
                }
            }
            return -1;
        } else {
            std::cout << "No such heuristic found: " << heuristic << std::endl;
            exit(0);
        }
    }

private:
    static bool Fits(const std::vector<long double>& server_resources, const std::vector<long double>& vm_resources) {
        bool fits = true;
        for (int i = 0; i < server_resources.size(); ++i) {
            if (server_resources[i] + vm_resources[i] > 1) {
                fits = false;
                break;
            }
        }
        return fits;
    }
};