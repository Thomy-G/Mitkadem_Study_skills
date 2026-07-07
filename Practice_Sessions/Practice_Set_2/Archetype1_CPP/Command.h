#ifndef COMMAND_H
#define COMMAND_H

// Write your Command declarations and CommandDispatcher here:

#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class BufferSystem;

class Command {
public:
  virtual ~Command() = default;
  virtual void execute(vector<string> &args, BufferSystem &system) = 0;
};

class PushCommand : public Command {
  void execute(vector<string> &args, BufferSystem &system) override;
};

class PopCommand : public Command {
  void execute(vector<string> &args, BufferSystem &system);
};

class FilterCommand : public Command {
  void execute(vector<string> &args, BufferSystem &system) override;
};

class CommandDispacher {
private:
  map<string, unique_ptr<Command>> command_registry;

public:
  void register_command(const string &verb, unique_ptr<Command> cmd);
  void dispatch(const string &verb, const vector<string> &args,
                BufferSystem &system);
};

#endif // COMMAND_H
