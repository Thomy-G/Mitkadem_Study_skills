#ifndef LOGGERSYSTEM_H
#define LOGGERSYSTEM_H

#include <string>
#include <vector>
#include <map>

class LoggerSystem {
private:
    std::map<std::string, int> log_counts;
    std::vector<std::string> ignored_prefixes;

    std::vector<std::string> split(const std::string& s, char delim);

public:
    LoggerSystem();
    bool is_ignored(const std::string& level) const;
    void log(const std::string& level, const std::string& message);
    void print_count(const std::string& level) const;
    void clear();
};

#endif // LOGGERSYSTEM_H
