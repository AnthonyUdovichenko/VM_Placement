#pragma once

#include <iostream>
#include <vector>

#include "mishra_sahoo_method.hpp"

class Algorithms {
public:
    static int FirstFit(const std::vector<std::vector<long double>>& servers, const std::vector<long double>& vm) {
        for (int i = 0; i < servers.size(); ++i) {
            if (Fits(servers[i], vm)) {
                return i;
            }
        }
        return -1;
    }

    static int BestFit(const std::string& heuristic, const std::vector<std::vector<long double>>& servers, const std::vector<long double>& vm) {
        long double best_value = 0;
        int best_ind = -1;
        for (int i = 0; i < servers.size(); ++i) {
            if (Fits(servers[i], vm)) {
                long double value;
                if (heuristic == "sum") {
                    value = sum(servers[i]);
                } else if (heuristic == "prod") {
                    value = prod(servers[i]);
                } else if (heuristic == "cpu") {
                    value = servers[i][0];
                } else {
                    std::cout << "In BestFit" << std::endl;
                    std::cout << "No such heuristic: " << heuristic << std::endl;
                    exit(0);
                }
                if (value > best_value) {
                    best_value = value;
                    best_ind = i;
                }
            }
        }
        return best_ind;
    }

    static int MishraSahoo(const std::string& goal, const std::vector<std::vector<long double>>& servers, const std::vector<long double>& vm) {
        MishraSahooMethod msm(2 / 3.0, 1 / 3.0);
        if (goal == "loadbalance") {
            return msm.dynamicPlaceVM(servers, vm, LOAD_BALANCE);
        } else if (goal == "consolidate") {
            return msm.dynamicPlaceVM(servers, vm, CONSOLIDATE);
        } else {
            std::cout << "In MishraSahoo" << std::endl;
            std::cout << "No such goal: " << goal << std::endl;
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

    static long double sum(const std::vector<long double>& server_resources) {
        long double result = 0;
        for (auto resource : server_resources) {
            result += resource;
        }
        return result;
    }

    static long double prod(const std::vector<long double>& server_resources) {
        long double result = 1;
        for (auto resource : server_resources) {
            result *= resource;
        }
        return result;
    }
};