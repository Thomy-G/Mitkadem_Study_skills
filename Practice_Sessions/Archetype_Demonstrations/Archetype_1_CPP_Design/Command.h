#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <memory>
#include <map>

class LoggerSystem; // Forward declaration

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const std::vector<std::string>& args, LoggerSystem& system) = 0;
};

class LogCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, LoggerSystem& system) override;
};

class CountCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, LoggerSystem& system) override;
};

class ClearCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, LoggerSystem& system) override;
};

class CommandDispatcher {
private:
    std::map<std::string, std::unique_ptr<Command>> command_registry;

public:
    void register_command(const std::string& verb, std::unique_ptr<Command> cmd);
    void dispatch(const std::string& verb, const std::vector<std::string>& args, LoggerSystem& system);
};

#endif // COMMAND_H
