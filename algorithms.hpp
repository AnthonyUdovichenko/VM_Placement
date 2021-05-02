#pragma once

#include <iostream>
#include <vector>

class Algorithms {
public:
    static int FirstFit(const std::vector<std::vector<long double>>& servers, const std::vector<long double>& resources) {
        for (int i = 0; i < servers.size(); ++i) {
            if (Fits(servers[i], resources)) {
                return i;
            }
        }
        return -1;
    }

    static int BestFit(const std::string& heuristic, const std::vector<std::vector<long double>>& servers, const std::vector<long double>& resources) {
        long double best_value = 0;
        int best_ind = -1;
        for (int i = 0; i < servers.size(); ++i) {
            if (Fits(servers[i], resources)) {
                if (heuristic == "sum") {
                    long double value = sum(servers[i]);
                    if (value > best_value) {
                        best_value = value;
                        best_ind = i;
                    }
                } else {
                    std::cout << "For BestFit" << std::endl;
                    std::cout << "No such heuristic: " << heuristic << std::endl;
                    exit(0);
                }
            }
        }
        return best_ind;
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

    static long double sum(const std::vector<long double>& server_resources) {
        long double result = 0;
        for (auto resource : server_resources) {
            result += resource;
        }
        return result;
    }
};