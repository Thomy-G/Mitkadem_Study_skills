#include "LoggerSystem.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

std::vector<std::string> LoggerSystem::split(const std::string& s, char delim) {
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

LoggerSystem::LoggerSystem() {
    const char* env_filter = std::getenv("LOGGER_IGNORE_FILTER");
    if (env_filter) {
        ignored_prefixes = split(std::string(env_filter), ',');
    }
}

bool LoggerSystem::is_ignored(const std::string& level) const {
    for (const auto& prefix : ignored_prefixes) {
        if (level.rfind(prefix, 0) == 0) {
            return true;
        }
    }
    return false;
}

void LoggerSystem::log(const std::string& level, const std::string& message) {
    if (is_ignored(level)) {
        return;
    }

    log_counts[level]++;
    std::cout << "[" << level << "]: " << message << std::endl;

    if (level == "ERROR") {
        std::cout << "CRITICAL ALERT: " << message << std::endl;
    }
}

void LoggerSystem::print_count(const std::string& level) const {
    auto it = log_counts.find(level);
    int count = (it != log_counts.end()) ? it->second : 0;
    std::cout << level << " count: " << count << std::endl;
}

void LoggerSystem::clear() {
    log_counts.clear();
}
