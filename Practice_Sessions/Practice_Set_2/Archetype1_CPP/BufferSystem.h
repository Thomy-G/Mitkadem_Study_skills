#ifndef BUFFERSYSTEM_H
#define BUFFERSYSTEM_H

// Write your BufferSystem declarations here:
#include "Command.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class BufferSystem {
private:
  vector<int> stack;
  int filtered_val;
  bool filtering;

public:
  BufferSystem();
  bool is_filtered(int &value);
  void push(int value);
  int pop();
  void filter(int value);
  bool empty();
};

#endif // BUFFERSYSTEM_H
