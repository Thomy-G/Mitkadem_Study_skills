#include "Command.h"
#include "BufferSystem.h"
#include "Utils.h"
#include <iostream>
#include <vector>

// Write your Command implementations here:

using namespace std;

void PushCommand::execute(vector<string> &args, BufferSystem &system) {
  if (args.size() < 1)
    return;

  int val = 0;
  if (!parse_int(args[0], val)) {
    return;
  }

  if (system.is_filtered(val)) {
    return;
  }
  system.push(val);
  cout << "Value " << val << " pushed" << endl;
}

void PopCommand::execute(vector<string> &args, BufferSystem &system) {

  if (!system.empty()) {
    int out = system.pop();
    cout << "Value " << out << " popped" << endl;
  } else {
    cout << "Buffer empty" << endl;
  }
}

void FilterCommand::execute(vector<string> &args, BufferSystem &system) {
  if (args.size() < 1)
    return;

  int val = 0;
  if (!parse_int(args[0], val)) {
    return;
  }

  system.filter(val);
}