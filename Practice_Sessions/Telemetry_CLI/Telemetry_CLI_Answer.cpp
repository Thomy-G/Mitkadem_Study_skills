/*
 * Advanced Systems Programming (893210)
 * Telemetry CLI Design Question - Answer File
 * 
 * Note: The detailed architecture and design explanation can be found in Telemetry_CLI_Solution.md
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <map>
#include <numeric>
#include <cstdlib>

// ==========================================
// 1. Model Layer: TelemetrySystem
// ==========================================

class TelemetrySystem {
private:
    std::map<std::string, std::vector<int>> metrics;
    std::map<std::string, int> thresholds;
    std::vector<std::string> ignored_prefixes;

    // Helper to split a string by delimiter
    std::vector<std::string> split(const std::string& s, char delim) {
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

public:
    TelemetrySystem() {
        // Load the environment filter upon construction
        const char* env_filter = std::getenv("METRICS_IGNORE_FILTER");
        if (env_filter) {
            ignored_prefixes = split(std::string(env_filter), ',');
        }
    }

    // Check if the metric name matches any ignore prefix
    bool is_ignored(const std::string& name) const {
        for (const auto& prefix : ignored_prefixes) {
            // Checks if name starts with prefix
            if (name.rfind(prefix, 0) == 0) {
                return true;
            }
        }
        return false;
    }

    // Record a metric
    void track(const std::string& name, int value) {
        if (is_ignored(name)) {
            return;
        }

        metrics[name].push_back(value);
        std::cout << name << " updated to " << value << std::endl;

        // Check warning threshold
        auto it = thresholds.find(name);
        if (it != thresholds.end()) {
            int limit = it->second;
            if (value > limit) {
                std::cout << "WARNING: " << name << " exceeded " << limit 
                          << "! (current: " << value << ")" << std::endl;
            }
        }
    }

    // Print average of a metric
    void print_average(const std::string& name) const {
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

    // Set threshold warning
    void set_threshold(const std::string& name, int limit) {
        thresholds[name] = limit;
    }
};

// ==========================================
// 2. Command Pattern (OCP Enforcement)
// ==========================================

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const std::vector<std::string>& args, TelemetrySystem& system) = 0;
};

// Robust integer parsing helper
bool parse_int(const std::string& str, int& out_val) {
    try {
        size_t idx;
        out_val = std::stoi(str, &idx);
        return idx == str.size(); // Ensure the entire string was parsed
    } catch (...) {
        return false;
    }
}

// "track [metric_name] [value]"
class TrackCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, TelemetrySystem& system) override {
        if (args.size() < 2) return; // Silent validation failure
        
        const std::string& metric_name = args[0];
        int val = 0;
        if (!parse_int(args[1], val)) {
            return; // Ignore non-integer inputs silently
        }
        system.track(metric_name, val);
    }
};

// "average [metric_name]"
class AverageCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, TelemetrySystem& system) override {
        if (args.size() < 1) return; // Silent validation failure
        
        const std::string& metric_name = args[0];
        system.print_average(metric_name);
    }
};

// "threshold [metric_name] [limit]"
class ThresholdCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, TelemetrySystem& system) override {
        if (args.size() < 2) return; // Silent validation failure
        
        const std::string& metric_name = args[0];
        int limit = 0;
        if (!parse_int(args[1], limit)) {
            return; // Ignore non-integer limits silently
        }
        system.set_threshold(metric_name, limit);
    }
};

// ==========================================
// 3. Central Dispatcher
// ==========================================

class CommandDispatcher {
private:
    std::map<std::string, std::unique_ptr<Command>> command_registry;

public:
    void register_command(const std::string& verb, std::unique_ptr<Command> cmd) {
        command_registry[verb] = std::move(cmd);
    }

    void dispatch(const std::string& verb, const std::vector<std::string>& args, TelemetrySystem& system) {
        auto it = command_registry.find(verb);
        if (it != command_registry.end()) {
            it->second->execute(args, system);
        }
        // Unrecognized verbs are ignored silently as per constraints
    }
};

// ==========================================
// 4. Main Event Loop
// ==========================================

int main() {
    TelemetrySystem system;
    CommandDispatcher dispatcher;

    // Register commands. Handlers are decoupled and easily extendable (OCP).
    dispatcher.register_command("track", std::make_unique<TrackCommand>());
    dispatcher.register_command("average", std::make_unique<AverageCommand>());
    dispatcher.register_command("threshold", std::make_unique<ThresholdCommand>());

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string verb;
        ss >> verb;

        std::string token;
        std::vector<std::string> args;
        while (ss >> token) {
            args.push_back(token);
        }

        dispatcher.dispatch(verb, args, system);
    }

    return 0;
}
