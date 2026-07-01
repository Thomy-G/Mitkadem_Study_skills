#ifndef TELEMETRYSYSTEM_H
#define TELEMETRYSYSTEM_H

#include <string>
#include <vector>
#include <map>

class TelemetrySystem {
private:
    std::map<std::string, std::vector<int>> metrics;
    std::map<std::string, int> thresholds;
    std::vector<std::string> ignored_prefixes;

    std::vector<std::string> split(const std::string& s, char delim);

public:
    TelemetrySystem();
    bool is_ignored(const std::string& name) const;
    void track(const std::string& name, int value);
    void print_average(const std::string& name) const;
    void set_threshold(const std::string& name, int limit);
};

#endif // TELEMETRYSYSTEM_H
