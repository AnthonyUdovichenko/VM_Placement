#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

enum Utilization {
    HIGH,
    MEDIUM,
    LOW
};

enum Goal {
    LOAD_BALANCE,
    CONSOLIDATE
};

class MishraSahooMethod {
public:
    MishraSahooMethod(long double high, long double medium) : high(high), medium(medium) {}

    int dynamicPlaceVM(const std::vector<std::vector<long double>>& servers_resources, const std::vector<long double>& vm, Goal goal) {
        std::vector<int> PotentialPMlist = getPotentialPMs(servers_resources, vm, goal);
        if (PotentialPMlist.empty()) {
            return -1;
        }
        std::vector<std::pair<long double, int>> M;
        std::vector<long double> VM_RIV = RIV(vm);
        for (auto PotentialPM : PotentialPMlist) {
            std::vector<long double> RIV_sum = RIV(servers_resources[PotentialPM]);
            for (int i = 0; i < 3; ++i) {
                RIV_sum[i] += VM_RIV[i];
            }
            M.emplace_back(std::sqrt(std::pow(RIV_sum[0], 2) + std::pow(RIV_sum[1], 2) + std::pow(RIV_sum[2], 2)), PotentialPM);
        }
        long double min_M = M[0].first;
        int min_M_ind = M[0].second;
        for (int i = 1; i < M.size(); ++i) {
            if (M[i].first < min_M) {
                min_M = M[i].first;
                min_M_ind = M[i].second;
            }
        }
        return min_M_ind;
    }

private:
    std::vector<int> getPotentialPMs(const std::vector<std::vector<long double>>& servers_resources, const std::vector<long double>& vm, Goal goal) {
        std::string T = CRT3(vm);
        std::vector<int> PotentialPMlist;
        if (goal == LOAD_BALANCE) {
            /// PMs in T which have low utilization
            for (int i = 0; i < servers_resources.size(); ++i) {
                if (RT3(servers_resources[i]) == T && getOverallPMUtilization(servers_resources[i]) == LOW) {
                    if (Fits(servers_resources[i], vm)) {
                        PotentialPMlist.push_back(i);
                    }
                }
            }

            if (PotentialPMlist.empty()) {
                std::string first_order_one = T;
                std::swap(first_order_one[0], first_order_one[1]);
                std::string first_order_two = T;
                std::swap(first_order_two[1], first_order_two[2]);
                for (int i = 0; i < servers_resources.size(); ++i) {
                    std::string rt3 = RT3(servers_resources[i]);
                    Utilization util = getOverallPMUtilization(servers_resources[i]);

                    /// PMs in T which have medium utilization
                    /// PMs in T's two first order neighbors which have low utilization
                    if (rt3 == T && util == MEDIUM ||
                        (rt3 == first_order_one || rt3 == first_order_two) && util == LOW) {
                        if (Fits(servers_resources[i], vm)) {
                            PotentialPMlist.push_back(i);
                        }
                    }
                }
            }

            if (PotentialPMlist.empty()) {
                std::string first_order_one = T;
                std::swap(first_order_one[0], first_order_one[1]);
                std::string first_order_two = T;
                std::swap(first_order_two[1], first_order_two[2]);

                std::string second_order_one = T;
                std::swap(second_order_one[0], second_order_one[2]);
                std::swap(second_order_one[0], second_order_one[1]);
                std::string second_order_two = T;
                std::swap(second_order_two[0], second_order_two[2]);
                std::swap(second_order_two[1], second_order_two[2]);

                for (int i = 0; i < servers_resources.size(); ++i) {
                    std::string rt3 = RT3(servers_resources[i]);
                    Utilization util = getOverallPMUtilization(servers_resources[i]);

                    /// PMs in T which have high utilization
                    /// PMs in T's two first order neighbors which have medium utilization
                    /// PMs in T's two second order neighbors which have low utilization
                    if (rt3 == T && util == HIGH ||
                        (rt3 == first_order_one || rt3 == first_order_two) && util == MEDIUM ||
                        (rt3 == second_order_one || rt3 == second_order_two) && util == LOW) {
                        if (Fits(servers_resources[i], vm)) {
                            PotentialPMlist.push_back(i);
                        }
                    }
                }
            }
        } else if (goal == CONSOLIDATE) {
            /// PMs in T which have high utilization
            for (int i = 0; i < servers_resources.size(); ++i) {
                if (RT3(servers_resources[i]) == T && getOverallPMUtilization(servers_resources[i]) == HIGH) {
                    if (Fits(servers_resources[i], vm)) {
                        PotentialPMlist.push_back(i);
                    }
                }
            }

            if (PotentialPMlist.empty()) {
                std::string first_order_one = T;
                std::swap(first_order_one[0], first_order_one[1]);
                std::string first_order_two = T;
                std::swap(first_order_two[1], first_order_two[2]);
                for (int i = 0; i < servers_resources.size(); ++i) {
                    std::string rt3 = RT3(servers_resources[i]);
                    Utilization util = getOverallPMUtilization(servers_resources[i]);

                    /// PMs in T which have medium utilization
                    /// PMs in T's two first order neighbors which have high utilization
                    if (rt3 == T && util == MEDIUM ||
                        (rt3 == first_order_one || rt3 == first_order_two) && util == HIGH) {
                        if (Fits(servers_resources[i], vm)) {
                            PotentialPMlist.push_back(i);
                        }
                    }
                }
            }

            if (PotentialPMlist.empty()) {
                std::string first_order_one = T;
                std::swap(first_order_one[0], first_order_one[1]);
                std::string first_order_two = T;
                std::swap(first_order_two[1], first_order_two[2]);

                std::string second_order_one = T;
                std::swap(second_order_one[0], second_order_one[2]);
                std::swap(second_order_one[0], second_order_one[1]);
                std::string second_order_two = T;
                std::swap(second_order_two[0], second_order_two[2]);
                std::swap(second_order_two[1], second_order_two[2]);

                for (int i = 0; i < servers_resources.size(); ++i) {
                    std::string rt3 = RT3(servers_resources[i]);
                    Utilization util = getOverallPMUtilization(servers_resources[i]);

                    /// PMs in T which have low utilization
                    /// PMs in T's two first order neighbors which have medium utilization
                    /// PMs in T's two second order neighbors which have high utilization
                    if (rt3 == T && util == LOW ||
                        (rt3 == first_order_one || rt3 == first_order_two) && util == MEDIUM ||
                        (rt3 == second_order_one || rt3 == second_order_two) && util == HIGH) {
                        if (Fits(servers_resources[i], vm)) {
                            PotentialPMlist.push_back(i);
                        }
                    }
                }
            }
        } else {
            std::cout << "getPotentialPMs(): Something went wrong, no such goal" << std::endl;
            exit(0);
        }

        if (PotentialPMlist.empty()) {
            for (int i = 0; i < servers_resources.size(); ++i) {
                if (Fits(servers_resources[i], vm)) {
                    PotentialPMlist.push_back(i);
                }
            }
        }
        return PotentialPMlist;
    }

    [[nodiscard]]
    Utilization getOverallPMUtilization(const std::vector<long double>& server_resources) const {
        long double maximum = max3(server_resources);
        if (maximum >= high) {
            return HIGH;
        }
        if (maximum >= medium) {
            return MEDIUM;
        }
        return LOW;
    }

    static std::vector<long double> RIV(const std::vector<long double>& RUV) {
        long double multiplier = (RUV[0] + RUV[1] + RUV[3]) / 3.0;
        std::vector<long double> result = {multiplier * RUV[0], multiplier * RUV[1], multiplier * RUV[2]};
        for (int i = 0; i < 3; ++i) {
            result[i] -= RUV[i];
        }
        return result;
    }

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

    static std::string CRT3(const std::vector<long double>& resources) {
        std::string result = RT3(resources);
        std::swap(result[0], result[2]);
        return result;
    }

    static std::string RT3(const std::vector<long double>& resources) {
        std::vector<std::pair<long double, std::string>> process;
        process.emplace_back(resources[0], "1");
        process.emplace_back(resources[1], "2");
        process.emplace_back(resources[2], "3");
        std::sort(process.begin(), process.end(), std::greater<>());
        std::string result = process[0].second + process[1].second + process[2].second;
        return result;
    }

    static long double max3(const std::vector<long double>& server_resources) {
        long double maximum = 0;
        for (int i = 0; i < 3; ++i) {
            maximum = std::max(maximum, server_resources[i]);
        }
        return maximum;
    }

    const long double high;
    const long double medium;
};