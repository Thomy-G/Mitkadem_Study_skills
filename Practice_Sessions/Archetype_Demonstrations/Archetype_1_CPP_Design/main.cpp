#include "LoggerSystem.h"
#include "Command.h"
#include <iostream>
#include <sstream>

int main() {
    LoggerSystem system;
    CommandDispatcher dispatcher;

    // Register all commands to achieve Open-Closed Principle compliance
    dispatcher.register_command("log", std::make_unique<LogCommand>());
    dispatcher.register_command("count", std::make_unique<CountCommand>());
    dispatcher.register_command("clear", std::make_unique<ClearCommand>());

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
