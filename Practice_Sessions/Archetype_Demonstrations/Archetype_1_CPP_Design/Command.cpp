#include "Command.h"
#include "LoggerSystem.h"
#include <numeric>

void LogCommand::execute(const std::vector<std::string>& args, LoggerSystem& system) {
    if (args.size() < 2) return; // Silent discard

    const std::string& level = args[0];
    
    // Concatenate the remaining elements as the message
    std::string message = args[1];
    for (size_t i = 2; i < args.size(); ++i) {
        message += " " + args[i];
    }

    system.log(level, message);
}

void CountCommand::execute(const std::vector<std::string>& args, LoggerSystem& system) {
    if (args.size() < 1) return; // Silent discard

    const std::string& level = args[0];
    system.print_count(level);
}

void ClearCommand::execute(const std::vector<std::string>& args, LoggerSystem& system) {
    // No arguments needed
    system.clear();
}

void CommandDispatcher::register_command(const std::string& verb, std::unique_ptr<Command> cmd) {
    command_registry[verb] = std::move(cmd);
}

void CommandDispatcher::dispatch(const std::string& verb, const std::vector<std::string>& args, LoggerSystem& system) {
    auto it = command_registry.find(verb);
    if (it != command_registry.end()) {
        it->second->execute(args, system);
    }
}
