#include "TelemetrySystem.h"
#include <iostream>
#include <sstream>
#include <numeric>
#include <cstdlib>

std::vector<std::string> TelemetrySystem::split(const std::string& s, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            tokens.push_back(item);
        }
    }
    return tokens;
}

TelemetrySystem::TelemetrySystem() {
    const char* env_filter = std::getenv("METRICS_IGNORE_FILTER");
    if (env_filter) {
        ignored_prefixes = split(std::string(env_filter), ',');
    }
}

bool TelemetrySystem::is_ignored(const std::string& name) const {
    for (const auto& prefix : ignored_prefixes) {
        if (name.rfind(prefix, 0) == 0) {
            return true;
        }
    }
    return false;
}

void TelemetrySystem::track(const std::string& name, int value) {
    if (is_ignored(name)) {
        return;
    }

    metrics[name].push_back(value);
    std::cout << name << " updated to " << value << std::endl;

    auto it = thresholds.find(name);
    if (it != thresholds.end()) {
        int limit = it->second;
        if (value > limit) {
            std::cout << "WARNING: " << name << " exceeded " << limit 
                      << "! (current: " << value << ")" << std::endl;
        }
    }
}

void TelemetrySystem::print_average(const std::string& name) const {
    auto it = metrics.find(name);
    if (it == metrics.end() || it->second.empty()) {
        std::cout << name << " has no records" << std::endl;
        return;
    }

    const auto& values = it->second;
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    double avg = sum / values.size();
    std::cout << name << " average: " << avg << std::endl;
}

void TelemetrySystem::set_threshold(const std::string& name, int limit) {
    thresholds[name] = limit;
}
