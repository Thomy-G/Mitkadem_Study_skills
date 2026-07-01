#include "TelemetrySystem.h"
#include "Command.h"
#include <iostream>
#include <sstream>

int main() {
    TelemetrySystem system;
    CommandDispatcher dispatcher;

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
