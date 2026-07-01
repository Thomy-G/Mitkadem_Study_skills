#include "Command.h"
#include "TelemetrySystem.h"
#include <sstream>

// Robust integer parsing helper
static bool parse_int(const std::string& str, int& out_val) {
    try {
        size_t idx;
        out_val = std::stoi(str, &idx);
        return idx == str.size();
    } catch (...) {
        return false;
    }
}

void TrackCommand::execute(const std::vector<std::string>& args, TelemetrySystem& system) {
    if (args.size() < 2) return;
    
    const std::string& metric_name = args[0];
    int val = 0;
    if (!parse_int(args[1], val)) {
        return;
    }
    system.track(metric_name, val);
}

void AverageCommand::execute(const std::vector<std::string>& args, TelemetrySystem& system) {
    if (args.size() < 1) return;
    
    const std::string& metric_name = args[0];
    system.print_average(metric_name);
}

void ThresholdCommand::execute(const std::vector<std::string>& args, TelemetrySystem& system) {
    if (args.size() < 2) return;
    
    const std::string& metric_name = args[0];
    int limit = 0;
    if (!parse_int(args[1], limit)) {
        return;
    }
    system.set_threshold(metric_name, limit);
}

void CommandDispatcher::register_command(const std::string& verb, std::unique_ptr<Command> cmd) {
    command_registry[verb] = std::move(cmd);
}

void CommandDispatcher::dispatch(const std::string& verb, const std::vector<std::string>& args, TelemetrySystem& system) {
    auto it = command_registry.find(verb);
    if (it != command_registry.end()) {
        it->second->execute(args, system);
    }
}
