#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class ServerPool {
public:
    explicit ServerPool(int resources_number) : resources_number(resources_number), vm_number(0), cpu_usage(0) {}

    [[nodiscard]]
    int GetResourcesNumber() const {
        return resources_number;
    }

    [[nodiscard]]
    const std::vector<std::vector<long double>>& GetServers() const {
        return servers;
    }

    unsigned int GetServersNumber() const {
        return servers.size();
    }

    int GetVMNumber() const {
        return vm_number;
    }

    long double GetCPUUsage() const {
        return cpu_usage;
    }

    void DeleteById(int vm_id) {
        if (id_to_server.count(vm_id) == 0) {
            return;
        }
        int server_id = id_to_server[vm_id];
        const std::vector<long double>& resources = id_to_resources[vm_id];
        for (int i = 0; i < resources_number; ++i) {
            servers[server_id][i] -= resources[i];
        }
        cpu_usage -= resources[0];
        id_to_resources.erase(vm_id);
        id_to_server.erase(vm_id);
        --vm_number;
    }

    void Place(int server_id, int vm_id, std::vector<long double> resources) {
        id_to_server[vm_id] = server_id;
        for (int i = 0; i < resources_number; ++i) {
            servers[server_id][i] += resources[i];
        }
        cpu_usage += resources[0];
        id_to_resources[vm_id] = std::move(resources);
        ++vm_number;
    }

    void AddServerAndPlace(int vm_id, std::vector<long double> resources) {
        servers.emplace_back(resources_number, 0);
        Place((int) servers.size() - 1, vm_id, std::move(resources));
    }

    void PrintServersSummaryToFile(const std::string& output_file) const {
        std::ofstream file(output_file);

        for (int i = 0; i < servers.size(); ++i) {
            file << i;
            for (auto value : servers[i]) {
                file << "," << value;
            }
            file << std::endl;
        }
    }

    void PrintServersNumberToFile(const std::string& output_file) const {
        std::ofstream file(output_file);

        file << servers.size() << std::endl;
    }

private:
    int resources_number;
    int vm_number;
    std::vector<std::vector<long double>> servers;
    std::unordered_map<int, std::vector<long double>> id_to_resources;
    std::unordered_map<int, int> id_to_server;
    long double cpu_usage;
};